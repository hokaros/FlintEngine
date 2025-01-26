#pragma once
#include <vector>
#include <Math/Vector.h>
#include <Math/GeometryStructures.h>

class VertexCollection
{
public:
	using iterator = std::vector<Vector>::iterator;

	struct EdgeCollectionId
	{
		EdgeCollectionId();
		explicit EdgeCollectionId(size_t index);

		size_t index;

		static const EdgeCollectionId INVALID;
	};

public:
	// returns the index of the new vertex
	size_t AddVertex(const Vector& v);
	void RemoveVertex(size_t index);
	void MergeVertices(size_t& v1, size_t& v2, const Vector& result_vertex);
	size_t GetNumVertices() const;

	iterator begin();
	iterator end();

	EdgeCollectionId CreateEdgeCollection();
	std::vector<IndexPair>& CreateAndGetEdgeCollection();
	std::vector<IndexPair>& GetEdgeCollection(EdgeCollectionId id);

	const std::vector<Vector>& GetVertices() const;
	// TODO: remove the below intrusive getter
	std::vector<Vector>& GetVertices();

	const Vector& operator[](size_t index) const;

private:
	void RemoveVertices(size_t& v1, size_t& v2);
	void ReattachEdges(size_t old_vertex1, size_t old_vertex2, size_t new_vertex);

private:
	std::vector<Vector> m_Vertices;
	std::vector<std::vector<IndexPair>> m_EdgeCollections;
};