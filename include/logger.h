#ifndef LOGGER_H
#define LOGGER_H

#include <stdio.h>
#include <stdlib.h>

#ifdef LOG_LEVEL_ALL
#define LOG_LEVEL_ERROR
#define LOG_LEVEL_WARNING
#define LOG_LEVEL_INFO
#define LOG_LEVEL_FATAL
#endif

#ifdef LOG_LEVEL_ERROR
#define LOG_ERROR(...) \
	printf("[ERROR]   "__VA_ARGS__)
#endif

#ifndef LOG_LEVEL_ERROR
#define LOG_ERROR(...)
#endif

#ifdef LOG_LEVEL_WARNING
#define LOG_WARNING(...) \
	printf("[WARNING] "__VA_ARGS__)
#endif

#ifndef LOG_LEVEL_WARNING
#define LOG_WARNING(...)
#endif

#ifdef LOG_LEVEL_INFO
#define LOG_INFO(...) \
	printf("[INFO]    "__VA_ARGS__)
#endif

#ifndef LOG_LEVEL_INFO
#define LOG_INFO(...)
#endif

#ifdef LOG_LEVEL_FATAL
#define LOG_FATAL(...) \
	do { \
		fprintf(stderr, "[FATAL] "__VA_ARGS__); \
		exit(1); \
	} while(0)
#endif

#ifndef LOG_LEVEL_FATAL
#define LOG_FATAL(...) \
	do { \
		exit(1); \
	} while(0)
#endif

#endif
