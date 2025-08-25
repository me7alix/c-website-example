#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

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

#include "impls.c"

typedef struct {
	char *db_path;
	short port;
} ServerConf;

ServerConf load_config(const char *path) {
	Conf conf = conf_parse(path);
	ServerConf config = {0};

	for (size_t i = 0; i < conf.count; i++) {
		if (strcmp(conf.keys[i], "DB_PATH") == 0) config.db_path = http_strdup(conf.values[i]);
		else if (strcmp(conf.keys[i], "PORT") == 0) config.port = atoi(conf.values[i]);
	}

	if (config.port == 0 || !config.db_path)
		LOG_FATAL("config loading error\n");

	conf_free(conf);
	return config;
}

void handler_index(void *ctx, HTTP_Request *req, HTTP_Response *resp) {
	IPostStorage *post_storage = (IPostStorage *) ctx;
	http_resp_add_header(resp, "Content-Type", CONTENT_TYPE_TEXT_HTML"; charset=utf-8");

	Post *posts;
	size_t posts_count;

	int err = post_storage->get_all(post_storage, &posts, &posts_count);
	if (err != STORAGE_ERR_OK) {
		http_resp_set_status_line(resp, STATUS_INTERNAL_SERVER_ERROR, "Internal server error");
		return;
	}

	char *page = tmpl_index(posts, posts_count);
	http_resp_set_body(resp, (uint8_t *)page, strlen(page));

	http_resp_set_status_line(resp, STATUS_OK, "OK");
}

int main(void) {
	ServerConf config = load_config("server.conf");

	IPostStorage *post_storage = sqlite_post_storage_create(config.db_path);
	if (!post_storage) {
		LOG_FATAL("database creating error\n");
	}

	if (post_storage->init(post_storage) != STORAGE_ERR_OK) {
		LOG_FATAL("database init error\n");
	}

	HTTP_Server serv = http_server_create(config.port);

	http_server_handle(&serv, "/", handler_index, post_storage);
	http_server_handle(&serv, "/post", handler_index, post_storage);

	if (http_server_serve_file(&serv, "/style.css", CONTENT_TYPE_TEXT_CSS, "./files/style.css") != 0) {
		LOG_ERROR("failed to register /style.css\n");
	}

	http_server_run(&serv);
	return 0;
}
