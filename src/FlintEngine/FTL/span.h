#pragma once
#include <vector>

namespace ftl
{
	template <typename T>
	class span
	{
	public:
		using iterator = typename std::vector<T>::const_iterator;

	public:
		constexpr span(const std::vector<T>& v)
			: m_Vector(v)
		{}

		constexpr const T& operator[](size_t index) const
		{
			return m_Vector[index];
		}

		constexpr size_t size() const
		{
			return m_Vector.size();
		}

		constexpr iterator begin() const
		{
			return m_Vector.begin();
		}

		constexpr iterator end() const
		{
			return m_Vector.end();
		}

	private:
		const std::vector<T>& m_Vector; // Temporary implementation, replace with pointer and size
	};
}