#include "NavmeshPath.h"

namespace Navigation
{
	void NavmeshPath::AddControlPoint(const Vector& point)
	{
		m_ControlPoints.emplace_back(point);
	}

	void NavmeshPath::Clear()
	{
		m_ControlPoints.clear();
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
}