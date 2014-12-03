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

#endif //__DEBUG_H_DEFINED__
