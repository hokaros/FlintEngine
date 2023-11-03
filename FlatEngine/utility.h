#pragma once
#include <stdio.h>

#define debug_break() __asm {int 3}

#ifdef _DEBUG

#define FE_ASSERT(cond, error_message)	\
		if (cond){}						\
		else							\
		{								\
			printf(error_message);		\
			debug_break();				\
		}								

#else

#define FE_ASSERT(cond)

#endif