#include <stdio.h>
#include <string.h>

#include "../include/storage.h"
#include "../include/logger.h"
#include "../thirdparty/http.h"

int get_posts(sqlite3 *db, const char *sql, Post **posts, size_t *out_count) {
	sqlite3_stmt *stmt;
	int capacity = 0;
	*out_count = 0;

	if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
		LOG_ERROR("failed to prepare statement: %s\n", sqlite3_errmsg(db));
		return STORAGE_ERR_INTERNAL;
	}

	while (sqlite3_step(stmt) == SQLITE_ROW) {
		if (*out_count >= capacity) {
			capacity = capacity == 0 ? 4 : capacity * 2;
			(*posts) = realloc((*posts), capacity * sizeof(Post));
		}

		(*posts)[*out_count].id = sqlite3_column_int(stmt, 0);

		const unsigned char *title = sqlite3_column_text(stmt, 1);
		(*posts)[*out_count].title = http_strdup((const char*)title);

		const unsigned char *text = sqlite3_column_text(stmt, 2);
		(*posts)[*out_count].html = http_strdup((const char*)text);
		(*out_count)++;
	}

	sqlite3_finalize(stmt);

	return STORAGE_ERR_NONE;
}

int sqlite_post_storage_init(IPostStorage *self) {
	PostStorage_SQLite *s = (PostStorage_SQLite *) self;

	const char *sql_create =
		"CREATE TABLE IF NOT EXISTS posts("
		"id INTEGER PRIMARY KEY AUTOINCREMENT,"
		"title TEXT NOT NULL,"
		"html TEXT NOT NULL);";

	char *errMsg = NULL;
	int rc = sqlite3_exec(s->db, sql_create, 0, 0, &errMsg);
	if (rc != SQLITE_OK) {
		LOG_ERROR("failed to init db: %s\n", errMsg);
		return STORAGE_ERR_INTERNAL;
	}

	return STORAGE_ERR_NONE;
}

int sqlite_post_storage_get(IPostStorage *self, int id, Post *post) {
	char buf[256];
	size_t out_count;

	PostStorage_SQLite *s = (PostStorage_SQLite *) self;
	sprintf(buf, "SELECT * FROM posts WHERE id = %d", id);

	Post *p;
	int err = get_posts(s->db, buf, &p, &out_count);
	if (err != STORAGE_ERR_NONE) {
		return err;
	}

	if (out_count == 0) {
		return STORAGE_ERR_NOT_FOUND;
	}

	*post = *p;
	return STORAGE_ERR_NONE;
}

int sqlite_post_storage_get_all(IPostStorage *self, Post **posts, size_t *out_count) {
	PostStorage_SQLite *s = (PostStorage_SQLite *) self;
	return get_posts(s->db, "SELECT * FROM posts", posts, out_count);
}

int sqlite_post_storage_add(IPostStorage *self, Post post) {
	PostStorage_SQLite *s = (PostStorage_SQLite *) self;

	char sql_insert_buf[256], *errMsg;
	sprintf(sql_insert_buf, "INSERT INTO posts (title, html) VALUES ('%s', '%s');", post.title, post.html);

	int rc = sqlite3_exec(s->db, sql_insert_buf, 0, 0, &errMsg);
	if (rc != SQLITE_OK) {
		LOG_ERROR("failed to add: %s\n", errMsg);
		return STORAGE_ERR_INTERNAL;
	}

	return STORAGE_ERR_NONE;
}

int sqlite_post_storage_remove(IPostStorage *self, int id) {
	PostStorage_SQLite *s = (PostStorage_SQLite *) self;

	char sql_insert_buf[256], *errMsg;
	sprintf(sql_insert_buf, "DELETE FROM posts WHERE id = %d;", id);

	int rc = sqlite3_exec(s->db, sql_insert_buf, 0, 0, &errMsg);
	if (rc != SQLITE_OK) {
		LOG_ERROR("failed to remove: %s\n", errMsg);
		return STORAGE_ERR_INTERNAL;
	}

	return 0;
}

IPostStorage *sqlite_post_storage_create(const char *db_path) {
	PostStorage_SQLite *res = malloc(sizeof(PostStorage_SQLite));
	*res = (PostStorage_SQLite){0};

	int rc = sqlite3_open(db_path, &res->db);
	if (rc != SQLITE_OK) {
		return NULL;
	}

	res->base.init = sqlite_post_storage_init;
	res->base.add = sqlite_post_storage_add;
	res->base.get = sqlite_post_storage_get;
	res->base.get_all = sqlite_post_storage_get_all;
	res->base.remove = sqlite_post_storage_remove;

	return (IPostStorage *) res;
}
