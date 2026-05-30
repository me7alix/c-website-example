#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define LOG_LEVEL_ALL

#include "../include/logger.h"
#include "../include/storage.h"

#define CONF_IMPLEMENTATION
#include "../thirdparty/conf.h"

#define HTTP_IMPLEMENTATION
#include "../thirdparty/http.h"

#define HTMPL_IMPLEMENTATION
#include "../thirdparty/htmpl.h"

char *tmpl_index(Post *posts, size_t cnt);
char *tmpl_admin(Post *posts, size_t cnt);

#include "impls.c"

typedef struct {
	char *db_path;
	short main_port;
	short admin_port;
} ServerConf;

ServerConf load_config(const char *path) {
	Conf conf = conf_parse(path);
	ServerConf config = {0};

	for (size_t i = 0; i < conf.count; i++) {
		if (strcmp(conf.keys[i], "DB_PATH") == 0) {
			config.db_path = http_strdup(conf.values[i]);
		} else if (strcmp(conf.keys[i], "MAIN_PORT") == 0) {
			config.main_port = atoi(conf.values[i]);
		} else if (strcmp(conf.keys[i], "ADMIN_PORT") == 0) {
			config.admin_port = atoi(conf.values[i]);
		}
	}

	if (config.main_port == 0 || config.admin_port == 0 || !config.db_path) {
		LOG_FATAL("config loading error\n");
	}

	conf_free(conf);
	return config;
}

void handler_index(void *ctx, HTTP_Request *req, HTTP_Response *resp) {
	IPostStorage *post_storage = (IPostStorage *) ctx;
	http_resp_add_header(resp, "Content-Type", CONTENT_TYPE_TEXT_HTML"; charset=utf-8");

	Post *posts;
	size_t posts_count;

	int err = post_storage->get_all(post_storage, &posts, &posts_count);
	if (err != STORAGE_ERR_NONE) {
		http_resp_set_status_line(resp, STATUS_INTERNAL_SERVER_ERROR, "Internal server error");
		return;
	}

	char *page = tmpl_index(posts, posts_count);
	http_resp_set_body(resp, (uint8_t *)page, strlen(page));

	http_resp_set_status_line(resp, STATUS_OK, "OK");
}

void handler_admin(void *ctx, HTTP_Request *req, HTTP_Response *resp) {
	IPostStorage *post_storage = (IPostStorage *) ctx;
	http_resp_add_header(resp, "Content-Type", CONTENT_TYPE_TEXT_HTML"; charset=utf-8");

	Post *posts;
	size_t posts_count;

	int err = post_storage->get_all(post_storage, &posts, &posts_count);
	if (err != STORAGE_ERR_NONE) {
		http_resp_set_status_line(resp, STATUS_INTERNAL_SERVER_ERROR, "Internal server error");
		return;
	}

	char *page = tmpl_admin(posts, posts_count);
	http_resp_set_body(resp, (uint8_t *)page, strlen(page));

	http_resp_set_status_line(resp, STATUS_OK, "OK");
}

void handler_post(void *ctx, HTTP_Request *req, HTTP_Response *resp) {
	IPostStorage *post_storage = (IPostStorage *) ctx;

	char buf[256];
	size_t tar_cnt = 0;

	memcpy(buf, req->target, sizeof(char) * strlen(req->target));
	char *tar_tok = strtok(buf, "/");

	while (tar_tok) {
		tar_cnt++;
		tar_tok = strtok(NULL, "/");
	}

	if (strcmp(req->method, METHOD_DELETE) == 0) {
		size_t tar_cur = 0; int id;
		memcpy(buf, req->target, sizeof(char) * strlen(req->target));
		tar_tok = strtok(buf, "/");

		while (tar_tok) {
			if (tar_cur++ == tar_cnt - 1)
				id = atoi(tar_tok);
			tar_tok = strtok(NULL, "/");
		}

		if (id == 0) {
			printf("post id error\n");
			http_resp_set_status_line(resp, STATUS_BAD_REQUEST, "post id was not provided");
			return;
		}

		int err = post_storage->remove(post_storage, id);
		if (err != STORAGE_ERR_NONE) {
			if (err == STORAGE_ERR_NOT_FOUND) {
				http_resp_set_status_line(resp, STATUS_BAD_REQUEST, "post with such id was not found");
				return;
			}

			http_resp_set_status_line(resp, STATUS_INTERNAL_SERVER_ERROR, "internal server error");
			return;
		}

		http_resp_set_status_line(resp, STATUS_OK, "OK");
	} else if (strcmp(req->method, METHOD_POST) == 0) {
		char title[256] = {0};
		char html[2048] = {0};

		for (size_t i = 0; i < req->body_len; i++) {
			if (req->body[i] == '\n') {
				memcpy(title, req->body, i * sizeof(char));
				memcpy(html, req->body + i + 1, (req->body_len - i - 1) * sizeof(char));
				title[i] = '\0';
				html[req->body_len - i - 1] = '\0';
				break;
			}
		}

		if (strlen(title) == 0 || strlen(html) == 0) {
			http_resp_set_status_line(resp, STATUS_BAD_REQUEST, "no post data");
			return;
		}

		int err = post_storage->add(post_storage, (Post) {.title = title, .html = html});
		if (err != STORAGE_ERR_NONE) {
			http_resp_set_status_line(resp, STATUS_INTERNAL_SERVER_ERROR, "internal server error");
			return;
		}

		http_resp_set_status_line(resp, STATUS_OK, "OK");
	} else {
		http_resp_set_status_line(resp, STATUS_METHOD_NOT_ALLOWED, "method not allowed");
	}
}

int main(void) {
	ServerConf config = load_config("server.conf");

	IPostStorage *post_storage = sqlite_post_storage_create(config.db_path);
	if (!post_storage) {
		LOG_FATAL("database creating error\n");
	}

	if (post_storage->init(post_storage) != STORAGE_ERR_NONE) {
		LOG_FATAL("database init error\n");
	}

	HTTP_Server main  = http_server_create(config.main_port);
	HTTP_Server admin = http_server_create(config.admin_port);

	http_server_handle(&main,  "/",      handler_index, post_storage);
	http_server_handle(&admin, "/post",  handler_post,  post_storage);
	http_server_handle(&admin, "/admin", handler_admin, post_storage);

	if (http_server_serve_file(&main, "/style.css", CONTENT_TYPE_TEXT_CSS, "./files/style.css") != 0) {
		LOG_ERROR("failed to register /style.css\n");
	}

	if (http_server_serve_file(&admin, "/style.css", CONTENT_TYPE_TEXT_CSS, "./files/style.css") != 0) {
		LOG_ERROR("failed to register /style.css\n");
	}

	if (fork()) {
		http_server_run(&main);
	} else {
		http_server_run(&admin);
	}

	return 0;
}
