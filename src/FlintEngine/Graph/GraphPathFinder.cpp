#include "GraphPathFinder.h"

#include <utility.h>
#include <FTL/VectorOperations.h>

namespace graph
{
	std::vector<NodeId> GraphPathFinder::FindPath(const PositionGraph& graph, NodeId start_node, NodeId end_node)
	{
		GraphPathFinder path_finder(graph, start_node, end_node);

		return path_finder.FindPathImpl();
	}

	GraphPathFinder::GraphPathFinder(const PositionGraph& graph, NodeId start_node, NodeId end_node)
		: m_Graph(graph)
		, m_StartNode(start_node)
		, m_EndNode(end_node)
	{

	}

	std::vector<NodeId> GraphPathFinder::FindPathImpl()
	{
		auto calculate_priority = [this](const NodeId& node) -> float
		{
			return 1.0f / CalculateEstimatedDistanceThrough(node);
		};
		ftl::PriorityQueue<NodeId> prio_queue(calculate_priority);
		
		prio_queue.Enqueue(m_StartNode);

		SetDistanceToNode(m_StartNode, 0.f, m_StartNode);

		while (!prio_queue.IsEmpty())
		{
			NodeId removed_node = prio_queue.PeekFront();
			prio_queue.PopFront();

			if (removed_node == m_EndNode)
			{
				return GatherPath();
			}

			UpdateNeighbourDistances(removed_node);

			InsertNeighboursToQueue(removed_node, prio_queue);
		}

		// Path not found
		return std::vector<NodeId>();
	}

	const std::vector<NodeId>& GraphPathFinder::GetNeighbours(NodeId node) const
	{
		return m_Graph.GetNodeById(node)->GetNeighbours();
	}

	void GraphPathFinder::UpdateNeighbourDistances(NodeId pivot)
	{
		const Vector pivot_pos = m_Graph.GetNodeById(pivot)->GetPos();
		const float from_start_to_pivot = GetDistanceToNode(pivot);

		for (NodeId neighbour : GetNeighbours(pivot))
		{
			const Vector neighbour_pos = m_Graph.GetNodeById(neighbour)->GetPos();
			const float from_pivot_to_neighbour = (neighbour_pos - pivot_pos).Length();

			const float new_distance = from_start_to_pivot + from_pivot_to_neighbour;
			if (new_distance < GetDistanceToNode(neighbour))
			{
				SetDistanceToNode(neighbour, new_distance, pivot);
			}
		}
	}

	void GraphPathFinder::InsertNeighboursToQueue(NodeId node, ftl::PriorityQueue<NodeId>& queue)
	{
		for (NodeId neighbour : GetNeighbours(node))
		{
			if (!IsNodeVisited(neighbour))
			{
				queue.Enqueue(neighbour);
				SetNodeVisited(neighbour);
			}
		}
	}

	float GraphPathFinder::CalculateEstimatedDistanceThrough(NodeId node) const
	{
		const float distance_to_node = GetDistanceToNode(node);
		const float est_from_node = EstimateDistanceToEndFrom(node);
		return distance_to_node + est_from_node;
	}

	float GraphPathFinder::GetDistanceToNode(NodeId node) const
	{
		auto it = m_DistancesToNodes.find(node);
		if (it == m_DistancesToNodes.end())
			return INFINITY;

		return it->second.distance;
	}

	void GraphPathFinder::SetDistanceToNode(NodeId node, float distance, NodeId prev_node)
	{
		m_DistancesToNodes[node] = NodeDistance(distance, prev_node);
	}

	bool GraphPathFinder::IsNodeVisited(NodeId node) const
	{
		auto it = m_VisitedNodes.find(node);
		if (it == m_VisitedNodes.end())
			return false;

		return true;
	}

	void GraphPathFinder::SetNodeVisited(NodeId node)
	{
		m_VisitedNodes.insert(node);
	}

	float GraphPathFinder::EstimateDistanceToEndFrom(NodeId node) const
	{
		const Vector& end_pos = m_Graph.GetNodeById(m_EndNode)->GetPos();
		const Vector& target_pos = m_Graph.GetNodeById(node)->GetPos();
		return (end_pos - target_pos).Length();
	}

	std::vector<NodeId> GraphPathFinder::GatherPath() const
	{
		std::vector<NodeId> path;

		NodeId curr_node = m_EndNode;
		while (curr_node != m_StartNode)
		{
			path.push_back(curr_node);

			NodeId next_node = m_DistancesToNodes.find(curr_node)->second.prev_node;
			curr_node = next_node;
		}

		ftl::vector_reverse(path);
		return path;
	}
}