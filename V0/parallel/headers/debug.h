#ifndef __DEBUG_H_DEFINED__
#define __DEBUG_H_DEFINED__

#include <stdio.h>
#include <errno.h>
#include <string.h>

#ifdef DEBUG_MODE
	#define debug(message, ...) fprintf(stderr, "Debug Info: %s:%d " message "\n", __FILE__, __LINE__, ##__VA_ARGS__)
#else
	#define debug(message, ...)
#endif

#define SafeMalloc(size, ...) __SafeMalloc(size, __FILE__, __LINE__)
void* __SafeMalloc(size_t size, const char* file, const int line);

#define Safefopen(path, mode, ...) __Safefopen(path, mode, __FILE__, __LINE__)
FILE* __Safefopen(const char* path, const char* mode, const char* file, const int line);

#endif //__DEBUG_H_DEFINED__
