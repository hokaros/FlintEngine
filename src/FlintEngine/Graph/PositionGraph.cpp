#include "PositionGraph.h"

namespace graph
{
	NodeId NodeId::INVALID = NodeId(std::numeric_limits<size_t>::max());

	PositionNode::PositionNode(NodeId id, const Vector& position)
		: m_Id(id)
		, m_Position(position)
	{
	}

	NodeId PositionNode::GetId() const
	{
		return m_Id;
	}

	void PositionNode::AddLink2Side(PositionNode& other)
	{
		AddLinkTo(other);
		other.AddLinkTo(*this);
	}

	void PositionNode::AddLinkTo(PositionNode& other)
	{
		m_Neighbours.push_back(other.m_Id);
	}

	PositionNode& PositionGraph::CreateNode(const Vector& position)
	{
		NodeId next_node_id = NodeId(m_Nodes.size());
		return m_Nodes.emplace_back(next_node_id, position);
	}

	PositionNode* PositionGraph::GetNodeById(NodeId id)
	{
		const PositionGraph* const_this = this;
		return const_cast<PositionNode*>(const_this->GetNodeById(id));
	}

	const PositionNode* PositionGraph::GetNodeById(NodeId id) const
	{
		const size_t index = id.id;
		if (index >= m_Nodes.size())
			return nullptr;

		return &m_Nodes[index];
	}

	bool NodeId::operator==(const NodeId& other) const
	{
		return id == other.id;
	}
}