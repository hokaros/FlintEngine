#pragma once
#include <vector>

namespace ftl
{
	template<typename T>
	bool vector_contains(const std::vector<T>& vec, const T& wanted_value)
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
	void vector_remove(std::vector<T>& vec, const T& value_to_remove)
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

	template<typename T, typename PredT>
	void vector_remove_if(std::vector<T>& vec, PredT&& predicate)
	{
		for (auto it = vec.begin(); it != vec.end();)
		{
			const T& vec_val = *it;

			if (predicate(vec_val))
			{
				it = vec.erase(it);
			}
			else
			{
				it++;
			}
		}
	}

	template<typename T>
	void vector_reverse(std::vector<T>& vec)
	{
		auto it = vec.begin();
		auto rev_it = vec.rbegin();

		while (it != rev_it.base())
		{
			T from_rev = *rev_it;
			*rev_it = *it;
			*it = from_rev;

			it++;
			if (it == rev_it.base())
			{
				break;
			}
			rev_it++;
		}
	}
}