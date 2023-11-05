#pragma once
#include <stdio.h>

#ifdef WIN32
#define debug_break() __asm {int 3}
#else
#define debug_break() __debugbreak()
#endif

#ifdef _DEBUG

#define FE_ASSERT(cond, error_message)	\
		if (cond){}						\
		else							\
		{								\
			printf(error_message);		\
			printf("\n");				\
			debug_break();				\
		}								

#else

#define FE_ASSERT(cond)

#endif