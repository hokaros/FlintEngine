#pragma once
#include "span.h"

namespace ftl
{
	template <typename T>
	class dyn_array2
	{
	public:
		constexpr dyn_array2(size_t width, size_t height)
			: m_Width(width)
			, m_Height(height)
			, m_Data(nullptr)
		{
			m_Data = new T[width * height];
		}

		~dyn_array2()
		{
			delete[] m_Data;
		}

		constexpr void set_all(const T& val)
		{
			for (size_t x = 0; x < m_Width; x++)
			{
				for (size_t y = 0; y < m_Height; y++)
				{
					at(x, y) = val;
				}
			}
		}

		constexpr T& at(size_t x, size_t y)
		{
			return m_Data[index_of_xy(x, y)];
		}

		constexpr const T& at(size_t x, size_t y) const
		{
			return m_Data[index_of_xy(x, y)];
		}

		constexpr size_t width() const
		{
			return m_Width;
		}

		constexpr size_t height() const
		{
			return m_Height;
		}

	private:
		constexpr size_t index_of_xy(size_t x, size_t y) const
		{
			return index_of_row_start(y) + x;
		}

		constexpr size_t index_of_row_start(size_t row_y) const
		{
			return m_Width * row_y;
		}

	private:
		T* m_Data;
		size_t m_Width;
		size_t m_Height;
	};
}