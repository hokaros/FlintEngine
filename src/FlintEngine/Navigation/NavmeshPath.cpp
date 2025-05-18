#include "NavmeshPath.h"

namespace Navigation
{
	NavmeshPath::NavmeshPath(NavmeshPath&& other)
		: m_ControlPoints(std::move(other.m_ControlPoints))
	{
	}

	void NavmeshPath::AddControlPoint(const Vector& point)
	{
		m_ControlPoints.emplace_back(point);
	}

	void NavmeshPath::Clear()
	{
		m_ControlPoints.clear();
	}

	bool NavmeshPath::IsEmpty() const
	{
		return m_ControlPoints.empty();
	}

	const Vector& NavmeshPath::GetFirstControlPoint() const
	{
		return m_ControlPoints.front();
	}

	void NavmeshPath::RemoveFirstControlPoint()
	{
		m_ControlPoints.erase(m_ControlPoints.begin());
	}

	const Vector& NavmeshPath::GetLastControlPoint() const
	{
		return m_ControlPoints.back();
	}

	size_t NavmeshPath::GetControlPointCount() const
	{
		return m_ControlPoints.size();
	}

	NavmeshPath::iterator NavmeshPath::begin()
	{
		return m_ControlPoints.begin();
	}

	NavmeshPath::iterator NavmeshPath::end()
	{
		return m_ControlPoints.end();
	}

	NavmeshPath::const_iterator NavmeshPath::begin() const
	{
		return m_ControlPoints.begin();
	}

	NavmeshPath::const_iterator NavmeshPath::end() const
	{
		return m_ControlPoints.end();
	}

	NavmeshPath& NavmeshPath::operator=(NavmeshPath&& other)
	{
		m_ControlPoints = std::move(other.m_ControlPoints);
		return *this;
	}

	const Vector& NavmeshPath::operator[](size_t index) const
	{
		return m_ControlPoints[index];
	}
}