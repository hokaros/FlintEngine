#pragma once
#include <vector>
#include <Math/Vector.h>

namespace Navigation
{
	class NavmeshPath
	{
	public:
		using iterator = std::vector<Vector>::iterator;
		using const_iterator = std::vector<Vector>::const_iterator;

		void AddControlPoint(const Vector& point);
		void Clear();

		iterator begin();
		iterator end();
		const_iterator begin() const;
		const_iterator end() const;

	private:
		std::vector<Vector> m_ControlPoints;
	};
}