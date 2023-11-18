#pragma once
#include <stdio.h>

#ifdef WIN32
#define debug_break() __asm {int 3}
#else
#define debug_break() __debugbreak()
#endif

#ifdef _DEBUG

#define FE_ASSERT(cond, error_message, ...)		\
		if (cond){}								\
		else									\
		{										\
			printf(error_message, __VA_ARGS__);	\
			printf("\n");						\
			debug_break();						\
		}								

#else

#define FE_ASSERT(cond)

#endif

#define FE_LOG(message, ...)			\
	printf(message, __VA_ARGS__);		\
	printf("\n");

#define FE_DATA_ERROR(message, ...)		\
	printf("ERROR: ");					\
	printf(message, __VA_ARGS__);		\
	printf("\n");