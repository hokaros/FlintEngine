#pragma once
#include <vector>
#include <map>
#include <set>

#include <FTL/PriorityQueue.h>
#include "PositionGraph.h"

namespace graph
{
	class GraphPathFinder
	{
	public:
		static std::vector<NodeId> FindPath(const PositionGraph& graph, NodeId start_node, NodeId end_node);

	private:
		struct NodeDistance
		{
			NodeDistance()
				: distance(INFINITY)
				, prev_node(NodeId::INVALID)
			{}

			NodeDistance(float distance, NodeId prev_node)
				: distance(distance)
				, prev_node(prev_node)
			{}

			float distance;
			NodeId prev_node;
		};

	private:
		GraphPathFinder(const PositionGraph& graph, NodeId start_node, NodeId end_node);

		std::vector<NodeId> FindPathImpl();

		const std::vector<NodeId>& GetNeighbours(NodeId node) const;
		void UpdateNeighbourDistances(NodeId node);
		void InsertNeighboursToQueue(NodeId node, ftl::PriorityQueue<NodeId>& queue);

		float CalculateEstimatedDistanceThrough(NodeId node) const;

		float GetDistanceToNode(NodeId node) const;
		void SetDistanceToNode(NodeId node, float distance, NodeId prev_node);
		bool IsNodeVisited(NodeId node) const;
		void SetNodeVisited(NodeId node);

		float EstimateDistanceToEndFrom(NodeId node) const;

		std::vector<NodeId> GatherPath() const;

	private:
		const PositionGraph& m_Graph;
		NodeId m_StartNode;
		NodeId m_EndNode;

		std::map<NodeId, NodeDistance> m_DistancesToNodes;
		std::set<NodeId> m_VisitedNodes;
	};
}