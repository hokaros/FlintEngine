#pragma once
#include <vector>

namespace ftl
{
	template<typename T>
	bool vector_contains(std::vector<T> vec, const T& wanted_value)
	{
		for (const T& vec_val : vec)
		{
			if (vec_val == wanted_value)
			{
				return true;
			}
		}

		return false;
	}
}