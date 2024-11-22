#include "pch.h"
#include <Graph/GraphPathFinder.h>

#ifdef SUITE_NAME
#error Cannot redefine suite name
#endif

#define SUITE_NAME GraphPathFinder

using graph::PositionGraph;
using graph::PositionNode;
using graph::GraphPathFinder;
using graph::NodeId;

TEST(SUITE_NAME, FindsDirectConnection)
{
	// Arrange
	PositionGraph graph;
	NodeId start_node = graph.CreateNode(Vector(0, 0));
	NodeId end_node = graph.CreateNode(Vector(5, 7));
	graph.AddLink2Side(start_node, end_node);

	// Act
	std::vector<NodeId> path = GraphPathFinder::FindPath(graph, start_node, end_node);

	// Assert
	ASSERT_EQ(1, path.size());
	ASSERT_EQ(1, path[0] == end_node);
}

TEST(SUITE_NAME, FindsLongPathWithoutBranching)
{
	// Arrange
	PositionGraph graph;
	NodeId start_node = graph.CreateNode(Vector(0, 0));

	NodeId node1 = graph.CreateNode(Vector(5, 7));
	graph.AddLink2Side(start_node, node1);

	NodeId node2 = graph.CreateNode(Vector(-2, 10));
	graph.AddLink2Side(node1, node2);

	NodeId node3 = graph.CreateNode(Vector(100, 1));
	graph.AddLink2Side(node2, node3);

	NodeId end_node = graph.CreateNode(Vector(50, 50));
	graph.AddLink2Side(node3, end_node);

	// Act
	std::vector<NodeId> path = GraphPathFinder::FindPath(graph, start_node, end_node);

	// Assert
	ASSERT_EQ(4, path.size());
	ASSERT_EQ(node1, path[0]);
	ASSERT_EQ(node2, path[1]);
	ASSERT_EQ(node3, path[2]);
	ASSERT_EQ(end_node, path[3]);
}

TEST(SUITE_NAME, FindsShorterPathInTripleBranch)
{
	// Arrange
	PositionGraph graph;
	NodeId start_node = graph.CreateNode(Vector(0, 0));

	NodeId branch1_point = graph.CreateNode(Vector(0, 10));
	graph.AddLink2Side(start_node, branch1_point);

	NodeId branch2_point1 = graph.CreateNode(Vector(1, 1));
	graph.AddLink2Side(start_node, branch2_point1);

	NodeId branch2_point2(graph.CreateNode(Vector(3, 3)));
	graph.AddLink2Side(branch2_point1, branch2_point2);

	NodeId branch3_point = graph.CreateNode(Vector(9, 0));
	graph.AddLink2Side(branch3_point, start_node);

	NodeId end_node = graph.CreateNode(Vector(10, 10));

	graph.AddLink2Side(end_node, branch1_point);
	graph.AddLink2Side(end_node, branch2_point2);
	graph.AddLink2Side(end_node, branch3_point);

	// Act
	std::vector<NodeId> path = GraphPathFinder::FindPath(graph, start_node, end_node);

	// Assert
	ASSERT_EQ(3, path.size());
	ASSERT_EQ(branch2_point1, path[0]);
	ASSERT_EQ(branch2_point2, path[1]);
	ASSERT_EQ(end_node, path[2]);
}

#undef SUITE_NAME