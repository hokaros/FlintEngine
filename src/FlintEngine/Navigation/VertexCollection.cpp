#include "VertexCollection.h"

void VertexCollection::AddVertex(const Vector& v)
{
	m_Vertices.push_back(v);
}

void VertexCollection::RemoveVertex(iterator it)
{
	const size_t removed_index = it - begin();

	for (std::vector<IndexPair>& edges : m_EdgeCollections)
	{
		for (auto it = edges.begin(); it != edges.end();)
		{
			IndexPair& edge = *it;
			if (edge.ContainsIndex(removed_index))
			{
				it = edges.erase(it);
			}
			else
			{
				if (edge.first > removed_index)
				{
					edge.first--;
				}
				if (edge.second > removed_index)
				{
					edge.second--;
				}

				it++;
			}
		}
	}

	m_Vertices.erase(it);
}

VertexCollection::iterator VertexCollection::begin()
{
	return m_Vertices.begin();
}

VertexCollection::iterator VertexCollection::end()
{
	return m_Vertices.end();
}

VertexCollection::EdgeCollectionId VertexCollection::CreateEdgeCollection()
{
	m_EdgeCollections.emplace_back();
	return EdgeCollectionId(m_EdgeCollections.size() - 1);
}

std::vector<IndexPair>& VertexCollection::CreateAndGetEdgeCollection()
{
	return GetEdgeCollection(CreateEdgeCollection());
}

std::vector<IndexPair>& VertexCollection::GetEdgeCollection(EdgeCollectionId id)
{
	const size_t index = id.index;

	return m_EdgeCollections[index];
}

std::vector<Vector>& VertexCollection::GetVertices()
{
	return m_Vertices;
}


VertexCollection::EdgeCollectionId::EdgeCollectionId(size_t index)
	: index(index)
{
}
