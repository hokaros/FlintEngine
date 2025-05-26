#include "NavmeshPathfinder.h"

#include <Graph/GraphPathFinder.h>

namespace Navigation
{
	void NavmeshPathfinder::FindPath(const Navmesh& navmesh, const Vector& start_point, const Vector& end_point, float min_width, NavmeshPath& out_path)
	{
		FindTriangleCenterPath(navmesh, start_point, end_point, out_path);
		CollapsePathControlPointsWhileClearSight(out_path, navmesh, min_width);
		PullPathControlPointsWhileClearSight(out_path, navmesh, min_width);
	}

	void NavmeshPathfinder::FindTriangleCenterPath(const Navmesh& navmesh, const Vector& start_point, const Vector& end_point, NavmeshPath& out_path)
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

	void NavmeshPathfinder::CollapsePathControlPointsWhileClearSight(NavmeshPath& path, const Navmesh& navmesh, float min_width)
	{
		if (path.GetControlPointCount() <= 2)
			return;

		NavmeshPath smoothed_path;
		smoothed_path.AddControlPoint(path[0]);
		size_t index_of_furthest_point_with_clean_sight = 1;

		while (true)
		{
			if (index_of_furthest_point_with_clean_sight == path.GetControlPointCount() - 1)
			{
				smoothed_path.AddControlPoint(path[index_of_furthest_point_with_clean_sight]);
				break;
			}

			const size_t next_index = index_of_furthest_point_with_clean_sight + 1;
			const Vector next_point = path[next_index];
			const Segment from_last_control_point_on_smooth_path_to_next_point = Segment(smoothed_path.GetLastControlPoint(), next_point);
			if (!navmesh.ContainsRect(DirectedRect(from_last_control_point_on_smooth_path_to_next_point, min_width)))
			{
				// No longer within sight, save last point
				smoothed_path.AddControlPoint(path[index_of_furthest_point_with_clean_sight]);
			}

			index_of_furthest_point_with_clean_sight++;
		}

		path = std::move(smoothed_path);
	}

	void NavmeshPathfinder::PullPathControlPointsWhileClearSight(NavmeshPath& path, const Navmesh& navmesh, float min_width)
	{
		if (path.GetControlPointCount() <= 2)
			return;

		for (size_t i = 1; i < path.GetControlPointCount() - 1; i++)
		{
			// Binary search for furthest pull that has clear sight
			Vector near_bound = path[i - 1];
			Vector far_bound = path[i];
			const Vector target = path[i + 1];

			for (size_t j = 0; j < s_MaxPullingIterationsPerSegment; j++)
			{
				const Vector mid = (near_bound + far_bound) / 2.0f;

				const Segment from_mid_to_target = Segment(mid, target);
				const bool is_sight_from_mid = navmesh.ContainsRect(DirectedRect(from_mid_to_target, min_width));

				if (is_sight_from_mid)
				{
					far_bound = mid;
				}
				else
				{
					near_bound = mid;
				}
			}

			path[i] = far_bound;
		}
	}
}