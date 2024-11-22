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

	const Vector& PositionNode::GetPos() const
	{
		return m_Position;
	}

	const std::vector<NodeId>& PositionNode::GetNeighbours() const
	{
		return m_Neighbours;
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

	NodeId PositionGraph::CreateNode(const Vector& position)
	{
		NodeId node_id = NodeId(m_Nodes.size());
		m_Nodes.emplace_back(node_id, position);

		return node_id;
	}

	PositionNode* PositionGraph::GetNodeById_NotConst(NodeId id)
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

	void PositionGraph::AddLink2Side(NodeId node_id1, NodeId node_id2)
	{
		GetNodeById_NotConst(node_id1)->AddLink2Side(*GetNodeById_NotConst(node_id2));
	}

	bool NodeId::operator==(const NodeId& other) const
	{
		return id == other.id;
	}

	bool NodeId::operator!=(const NodeId& other) const
	{
		return id != other.id;
	}

	bool NodeId::operator<(const NodeId& other) const
	{
		return id < other.id;
	}
}