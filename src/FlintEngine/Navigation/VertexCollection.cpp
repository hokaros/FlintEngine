#include "VertexCollection.h"

size_t VertexCollection::AddVertex(const Vector& v)
{
	m_Vertices.push_back(v);
	return m_Vertices.size() - 1;
}

void VertexCollection::RemoveVertex(size_t removed_index)
{
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

	m_Vertices.erase(m_Vertices.begin() + removed_index);
}

void VertexCollection::MergeVertices(size_t& v1, size_t& v2, const Vector& result_vertex)
{
	m_Vertices.push_back(result_vertex);

	const size_t merged_vertex_index = m_Vertices.size() - 1;

	ReattachEdges(v1, v2, merged_vertex_index);

	RemoveVertices(v1, v2);
}

size_t VertexCollection::GetNumVertices() const
{
	return m_Vertices.size();
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

const Vector& VertexCollection::operator[](size_t index) const
{
	return m_Vertices[index];
}

void VertexCollection::RemoveVertices(size_t& v1, size_t& v2)
{
	RemoveVertex(v1);
	if (v1 > 0)
	{
		v1--;
	}
	if (v2 > v1)
	{
		v2--;
	}

	RemoveVertex(v2);
	if (v2 > 0)
	{
		v2--;
	}
	if (v1 > v2)
	{
		v1--;
	}
}

void VertexCollection::ReattachEdges(size_t old_vertex1, size_t old_vertex2, size_t new_vertex)
{
	auto test_and_assign = [new_vertex](size_t& tested_in_link, size_t detached_vertex)
	{
		if (tested_in_link == detached_vertex)
		{
			tested_in_link = new_vertex;
		}
	};

	for (std::vector<IndexPair>& edges : m_EdgeCollections)
	{
		for (IndexPair& edge : edges)
		{
			test_and_assign(edge.first, old_vertex1);
			test_and_assign(edge.second, old_vertex1);
			test_and_assign(edge.first, old_vertex2);
			test_and_assign(edge.second, old_vertex2);
		}
	}
}


VertexCollection::EdgeCollectionId::EdgeCollectionId(size_t index)
	: index(index)
{
}
