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

	template<typename T>
	void vector_remove(std::vector<T> vec, const T& value_to_remove)
	{
		for (auto it = vec.begin(); it != vec.end(); it++)
		{
			const T& vec_val = *it;

			if (vec_val == value_to_remove)
			{
				vec.erase(it);
				return;
			}
		}
	}
}