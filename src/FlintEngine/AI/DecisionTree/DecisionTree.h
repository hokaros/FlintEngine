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
		DecisionTree<ResultT>(IDecisionTreeNode<ResultT>& rootNode);

		const ResultT& Decide() const;

	private:
		IDecisionTreeNode<ResultT>& m_RootNode;
	};



	template<typename ResultT>
	inline DecisionTree<ResultT>::DecisionTree(IDecisionTreeNode<ResultT>& rootNode)
		: m_RootNode(rootNode)
	{

	}

	template<typename ResultT>
	inline const ResultT& DecisionTree<ResultT>::Decide() const
	{
		return m_RootNode.Decide();
	}
}