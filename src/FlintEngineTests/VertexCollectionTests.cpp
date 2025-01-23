#include "pch.h"
#include <Navigation/VertexCollection.h>
#include "TestHelpers.h"

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
	vCollection.RemoveVertex(0);

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
	vCollection.RemoveVertex(0);

	// Assert
	ASSERT_EQ(1, edges.size());
	ASSERT_EQ(IndexPair(2, 0), edges[0]);
}

TEST(SUITE_NAME, MergingFreeVerticesKeepsOtherConnections)
{
	// Arrange
	VertexCollection vCollection;
	vCollection.AddVertex(Vector(0, 0));
	vCollection.AddVertex(Vector(0, 1));
	vCollection.AddVertex(Vector(1, 0));
	vCollection.AddVertex(Vector(1, 1));

	std::vector<IndexPair>& edges = vCollection.CreateAndGetEdgeCollection();
	const size_t attach_idx1 = 2;
	const Vector prev_attached_v1 = vCollection[attach_idx1];
	const size_t attach_idx2 = 3;
	const Vector prev_attached_v2 = vCollection[attach_idx2];
	edges.push_back(IndexPair(attach_idx1, attach_idx2));
	edges.push_back(IndexPair(attach_idx2, attach_idx1));

	// Act
	size_t merged_idx1 = 0;
	size_t merged_idx2 = 1;
	vCollection.MergeVertices(merged_idx1, merged_idx2, Vector(0, 0.5f));

	// Assert
	ASSERT_EQ(2, edges.size());
	ASSERT_VEC_EQ(prev_attached_v1, vCollection[edges[0].first]);
	ASSERT_VEC_EQ(prev_attached_v2, vCollection[edges[0].second]);
	ASSERT_VEC_EQ(prev_attached_v2, vCollection[edges[1].first]);
	ASSERT_VEC_EQ(prev_attached_v1, vCollection[edges[1].second]);
}

TEST(SUITE_NAME, MergingAttachedVerticesReconnects)
{
	// Arrange
	VertexCollection vCollection;
	vCollection.AddVertex(Vector(0, 0));
	vCollection.AddVertex(Vector(0, 1));
	vCollection.AddVertex(Vector(1, 1));
	vCollection.AddVertex(Vector(1, 0));

	std::vector<IndexPair>& edges = vCollection.CreateAndGetEdgeCollection();
	edges.push_back(IndexPair(0, 1));
	edges.push_back(IndexPair(3, 2));

	// Act
	size_t merged_idx1 = 0;
	size_t merged_idx2 = 3;
	const Vector merged_vertex = Vector(5, 5);
	vCollection.MergeVertices(merged_idx1, merged_idx2, merged_vertex);

	// Assert
	ASSERT_EQ(2, edges.size());
	ASSERT_VEC_EQ(merged_vertex, vCollection[edges[0].first]);
	ASSERT_VEC_EQ(merged_vertex, vCollection[edges[1].first]);
}

#undef SUITE_NAME