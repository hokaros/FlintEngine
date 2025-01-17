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
		explicit EdgeCollectionId(size_t index);

		size_t index;
	};

public:
	void AddVertex(const Vector& v);
	void RemoveVertex(iterator it);

	iterator begin();
	iterator end();

	EdgeCollectionId CreateEdgeCollection();
	std::vector<IndexPair>& CreateAndGetEdgeCollection();
	std::vector<IndexPair>& GetEdgeCollection(EdgeCollectionId id);

private:
	std::vector<Vector> m_Vertices;
	std::vector<std::vector<IndexPair>> m_EdgeCollections;
};