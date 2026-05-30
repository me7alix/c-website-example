#ifndef STORAGE_H
#define STORAGE_H

#include <stdlib.h>
#include <sqlite3.h>
#include "post.h"

typedef enum {
	STORAGE_ERR_NONE,
	STORAGE_ERR_NOT_FOUND,
	STORAGE_ERR_INTERNAL,
} StorageError;

typedef struct {
	int (*init)(void *self);
	int (*add)(void *self, Post post);
	int (*get)(void *self, int id, Post *post);
	int (*get_all)(void *self, Post **posts, size_t *out_count);
	int (*remove)(void *self, int id);
} IPostStorage;

typedef struct {
	IPostStorage base;
	sqlite3 *db;
} PostStorage_SQLite;

IPostStorage *sqlite_post_storage_create(const char *db_path);

#endif
