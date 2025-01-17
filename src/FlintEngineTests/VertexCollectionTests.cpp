#include "pch.h"
#include <Navigation/VertexCollection.h>

#ifdef SUITE_NAME
#error Cannot redefine suite name
#endif

#define SUITE_NAME VertexCollection

TEST(SUITE_NAME, RemovingFromStartAdjustsEdgeIndices)
{
	// Arrange
	VertexCollection vCollection;
	vCollection.AddVertex(Vector(0, 0));
	vCollection.AddVertex(Vector(0, 1));
	vCollection.AddVertex(Vector(1, 1));
	vCollection.AddVertex(Vector(1, 0));

	std::vector<IndexPair>& edges = vCollection.CreateAndGetEdgeCollection();
	edges.push_back(IndexPair(1, 2));
	edges.push_back(IndexPair(3, 1));

	// Act
	vCollection.RemoveVertex(vCollection.begin());

	// Assert
	ASSERT_EQ(IndexPair(0, 1), edges[0]);
	ASSERT_EQ(IndexPair(2, 0), edges[1]);
}


TEST(SUITE_NAME, RemovingVertexRemovesEdgesAndAdjustsIndices)
{
	// Arrange
	VertexCollection vCollection;
	vCollection.AddVertex(Vector(0, 0));
	vCollection.AddVertex(Vector(0, 1));
	vCollection.AddVertex(Vector(1, 1));
	vCollection.AddVertex(Vector(1, 0));

	std::vector<IndexPair>& edges = vCollection.CreateAndGetEdgeCollection();
	edges.push_back(IndexPair(0, 2));
	edges.push_back(IndexPair(3, 1));

	// Act
	vCollection.RemoveVertex(vCollection.begin());

	// Assert
	ASSERT_EQ(1, edges.size());
	ASSERT_EQ(IndexPair(2, 0), edges[0]);
}

#undef SUITE_NAME