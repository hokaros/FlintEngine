#pragma once

namespace dt
{
	template <typename ResultT>
	class IDecisionTreeNode
	{
	public:
		virtual const ResultT& Decide() const = 0;

		virtual ~IDecisionTreeNode() = default;
	};
}