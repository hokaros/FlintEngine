#pragma once
#include <memory>

#include <AI/DecisionTree/IDecisionTreeNode.h>
#include <utility.h>

namespace dt
{
	template <typename ResultT>
	class DecisionTree
	{
	public:
		DecisionTree<ResultT>(std::unique_ptr<IDecisionTreeNode<ResultT>> rootNode);

		const ResultT& Decide() const;

	private:
		std::unique_ptr<IDecisionTreeNode<ResultT>> m_RootNode;
	};



	template<typename ResultT>
	inline DecisionTree<ResultT>::DecisionTree(std::unique_ptr<IDecisionTreeNode<ResultT>> rootNode)
		: m_RootNode(std::move(rootNode))
	{
		FE_ASSERT(m_RootNode != nullptr, "Didn't pass root node to DecisionTree");
	}

	template<typename ResultT>
	inline const ResultT& DecisionTree<ResultT>::Decide() const
	{
		return m_RootNode->Decide();
	}
}