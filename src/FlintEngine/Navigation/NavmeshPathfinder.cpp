#include "NavmeshPathfinder.h"

#include <Graph/GraphPathFinder.h>

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

	void NavmeshPathfinder::FindPath(const Navmesh& navmesh, const Vector& start_point, const Vector& end_point, NavmeshPath& out_path)
	{
		const graph::NodeId start_triangle = navmesh.GetTriangleOfPos(start_point);
		const graph::NodeId end_triangle = navmesh.GetTriangleOfPos(end_point);
		if (start_triangle == graph::NodeId::INVALID || end_triangle == graph::NodeId::INVALID)
			return; // Either start pos or end pos is outside of navmesh

		if (start_triangle == end_triangle)
		{
			out_path.AddControlPoint(start_point);
			out_path.AddControlPoint(end_point);
			return;
		}

		const std::vector<graph::NodeId> path_on_graph = graph::GraphPathFinder::FindPath(navmesh.GetGraph(), start_triangle, end_triangle);
		if (path_on_graph.empty())
			return;

		out_path.AddControlPoint(start_point);
		for (auto it = path_on_graph.begin(); it != path_on_graph.end(); it++)
		{
			const bool is_first_node = it == path_on_graph.begin();
			const bool is_last_node = it + 1 == path_on_graph.end();
			if (!is_first_node && !is_last_node)
			{
				const graph::NodeId graph_node = *it;
				out_path.AddControlPoint(navmesh.GetGraph().GetNodeById(graph_node)->GetPos());
			}
		}
		out_path.AddControlPoint(end_point);
	}
}