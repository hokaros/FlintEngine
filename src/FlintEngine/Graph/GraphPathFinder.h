#pragma once
#include <vector>

#include "PositionGraph.h"

namespace graph
{
	class GraphPathFinder
	{
	public:
		GraphPathFinder(PositionGraph& graph);

		std::vector<NodeId> FindPath(NodeId start_node, NodeId end_node);

	private:
		PositionGraph& m_Graph;
	};
}