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

	public:
		NavmeshPath() = default;
		NavmeshPath(NavmeshPath&& other);

		void AddControlPoint(const Vector& point);
		void Clear();
		bool IsEmpty() const;
		const Vector& GetFirstControlPoint() const;
		void RemoveFirstControlPoint();
		const Vector& GetLastControlPoint() const;
		size_t GetControlPointCount() const;

		iterator begin();
		iterator end();
		const_iterator begin() const;
		const_iterator end() const;

		NavmeshPath& operator=(NavmeshPath&& other);
		const Vector& operator[](size_t index) const;

	private:
		std::vector<Vector> m_ControlPoints;
	};
}