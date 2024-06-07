#include <gtest/gtest.h>

#define ASSERT_VEC_EQ(expected, actual)		\
	ASSERT_FLOAT_EQ(expected.x, actual.x);	\
	ASSERT_FLOAT_EQ(expected.y, actual.y);