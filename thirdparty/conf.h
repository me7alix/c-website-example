#ifndef CONF_H
#define CONF_H

#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

typedef struct {
	char **keys;
	char **values;
	size_t count;
} Conf;

Conf conf_parse(const char *filename);
void conf_free(Conf conf);

#endif

#ifdef CONF_IMPLEMENTATION

Conf conf_parse(const char *filename) {
	FILE *file = fopen(filename, "r");
	if (file == NULL) {
		perror("conf.h: error opening file");
		exit(1);
	}

	Conf conf = {0};
	size_t conf_cap = 8;
	conf.keys = (char **) malloc(sizeof(char *) * conf_cap);
	conf.values = (char **) malloc(sizeof(char *) * conf_cap);


	char line[256];	
	while (fgets(line, sizeof(line), file)) {
		line[strlen(line)-1] = '\0';
		if (line[0] == '#') continue;

		char *key = 0, *value = 0;
		size_t cnt = 0, val_cap;
		char *tok = strtok(line, " ");

		while (tok != NULL) {
			if (cnt++ == 0) key = tok;
			else {
				if (!value) {
					value = (char *) malloc(sizeof(char) * (strlen(tok) + 32));
					val_cap = strlen(tok) + 32;
					strcat(value, tok);
				} else {
					if (val_cap <= strlen(value) + strlen(tok) + 2) {
						val_cap *= 2;
						value = (char *) realloc((void *) value, val_cap);
					}

					strcat(value, " ");
					strcat(value, tok);
				}
			}

			tok = strtok(NULL, " ");
		}

		if (cnt == 0) continue;
		else if (cnt == 1) {
			fprintf(stderr, "conf.h: parsing error\n");
			exit(1);
		}

		conf.keys[conf.count] = (char *) malloc(strlen(key) + 1);
		memcpy(conf.keys[conf.count], key, strlen(key) + 1);
		conf.values[conf.count++] = value;

		if (conf.count > conf_cap) {
			conf_cap *= 2;
			conf.keys = (char **) realloc((void *) conf.keys, sizeof(char *) * conf_cap);
			conf.values = (char **) realloc((void *) conf.values, sizeof(char *) * conf_cap);
		}
	}

	fclose(file);
	return conf;
}


void conf_free(Conf conf) {
	for (size_t i = 0; i < conf.count; i++) {
		free(conf.keys[i]);
		free(conf.values[i]);
	}

	free(conf.keys);
	free(conf.values);
}

#endif
