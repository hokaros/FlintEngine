#pragma once
#include <vector>
#include <Math/Vector.h>

namespace graph
{
	struct NodeId
	{
	public:
		constexpr NodeId(size_t id)
			: id(id)
		{}

		bool operator==(const NodeId& other) const;
		bool operator!=(const NodeId& other) const;
		bool operator<(const NodeId& other) const;

	public:
		size_t id;

		static NodeId INVALID;
	};

	class PositionNode
	{
	public:
		PositionNode(NodeId id, const Vector& position);

		NodeId GetId() const;
		const Vector& GetPos() const;
		const std::vector<NodeId>& GetNeighbours() const;

		void AddLink2Side(PositionNode& other);

	private:
		void AddLinkTo(PositionNode& other);

	private:
		NodeId m_Id = NodeId::INVALID;
		Vector m_Position;
		std::vector<NodeId> m_Neighbours;
	};

	class PositionGraph
	{
	public:
		NodeId CreateNode(const Vector& position);

		const PositionNode* GetNodeById(NodeId id) const;

		void AddLink2Side(NodeId node1, NodeId node2);

	private:
		PositionNode* GetNodeById_NotConst(NodeId id);

	private:
		std::vector<PositionNode> m_Nodes;
	};
}