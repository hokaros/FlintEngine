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

	public:
		size_t id;

		static NodeId INVALID;
	};

	class PositionNode
	{
	public:
		PositionNode(NodeId id, const Vector& position);

		NodeId GetId() const;

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
		PositionNode& CreateNode(const Vector& position);

		PositionNode* GetNodeById(NodeId id);
		const PositionNode* GetNodeById(NodeId id) const;

	private:
		std::vector<PositionNode> m_Nodes;
	};
}