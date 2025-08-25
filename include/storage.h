#ifndef STORAGE_H
#define STORAGE_H

#include <stdlib.h>
#include <sqlite3.h>
#include "post.h"

typedef enum {
	STORAGE_ERR_OK,
	STORAGE_ERR_NOT_FOUND,
	STORAGE_ERR_INTERNAL,
} StorageError;

typedef struct IPostStorage {
	int (*init)(struct IPostStorage *self);
	int (*add)(struct IPostStorage *self, Post post);
	int (*get)(struct IPostStorage *self, int id, Post *post);
	int (*get_all)(struct IPostStorage *self, Post **posts, size_t *out_count);
	int (*remove)(struct IPostStorage *self, int id);
} IPostStorage;

typedef struct {
	IPostStorage base;
	sqlite3 *db;
} PostStorage_SQLite;

IPostStorage *sqlite_post_storage_create(const char *db_path);

#endif
