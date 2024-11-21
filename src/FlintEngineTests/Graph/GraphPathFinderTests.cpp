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
	PositionNode& start_node = graph.CreateNode(Vector(0, 0));
	PositionNode& end_node = graph.CreateNode(Vector(5, 7));
	end_node.AddLink2Side(start_node);

	// Act
	GraphPathFinder pathfinder(graph);
	std::vector<NodeId> path = pathfinder.FindPath(start_node.GetId(), end_node.GetId());

	// Assert
	ASSERT_EQ(1, path.size());
	ASSERT_EQ(1, path[0] == end_node.GetId());
}

TEST(SUITE_NAME, FindsLongPathWithoutBranching)
{
	// Arrange
	PositionGraph graph;
	PositionNode& start_node = graph.CreateNode(Vector(0, 0));

	PositionNode& node1 = graph.CreateNode(Vector(5, 7));
	node1.AddLink2Side(start_node);

	PositionNode& node2 = graph.CreateNode(Vector(-2, 10));
	node2.AddLink2Side(node1);

	PositionNode& node3 = graph.CreateNode(Vector(100, 1));
	node3.AddLink2Side(node2);

	PositionNode& end_node = graph.CreateNode(Vector(50, 50));
	end_node.AddLink2Side(node3);

	// Act
	GraphPathFinder pathfinder(graph);
	std::vector<NodeId> path = pathfinder.FindPath(start_node.GetId(), end_node.GetId());

	// Assert
	ASSERT_EQ(4, path.size());
	ASSERT_EQ(node1.GetId(),  path[0]);
	ASSERT_EQ(node2.GetId(), path[1]);
	ASSERT_EQ(node3.GetId(), path[2]);
	ASSERT_EQ(end_node.GetId(), path[3]);
}

TEST(SUITE_NAME, FindsShorterPathInTripleBranch)
{
	// Arrange
	PositionGraph graph;
	PositionNode& start_node = graph.CreateNode(Vector(0, 0));

	PositionNode& branch1_point = graph.CreateNode(Vector(0, 10));
	branch1_point.AddLink2Side(start_node);

	PositionNode& branch2_point1 = graph.CreateNode(Vector(1, 1));
	branch2_point1.AddLink2Side(start_node);

	PositionNode& branch2_point2(graph.CreateNode(Vector(3, 3)));
	branch2_point2.AddLink2Side(branch2_point1);

	PositionNode& branch3_point = graph.CreateNode(Vector(9, 0));
	branch3_point.AddLink2Side(start_node);

	PositionNode& end_node = graph.CreateNode(Vector(10, 10));
	end_node.AddLink2Side(branch1_point);
	end_node.AddLink2Side(branch2_point2);
	end_node.AddLink2Side(branch3_point);

	// Act
	GraphPathFinder pathfinder(graph);
	std::vector<NodeId> path = pathfinder.FindPath(start_node.GetId(), end_node.GetId());

	// Assert
	ASSERT_EQ(3, path.size());
	ASSERT_EQ(branch2_point1.GetId(), path[0]);
	ASSERT_EQ(branch2_point2.GetId(), path[1]);
	ASSERT_EQ(end_node.GetId(), path[2]);
}

#undef SUITE_NAME