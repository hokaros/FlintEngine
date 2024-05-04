#pragma once

#include <AI/DecisionTree/DecisionTree.h>
#include <AI/DecisionTree/DecisionTreeSplit.h>
#include <AI/DecisionTree/DecisionTreeResult.h>

namespace dt
{
	template <typename DecisionResultT>
	class SplitBuilder
	{
	public:
		SplitBuilder<DecisionResultT>& Condition(std::unique_ptr<ICondition> condition);
		SplitBuilder<DecisionResultT>& True(std::unique_ptr<IDecisionTreeNode<DecisionResultT>> branch);
		SplitBuilder<DecisionResultT>& False(std::unique_ptr<IDecisionTreeNode<DecisionResultT>> branch);

		std::unique_ptr<DecisionTreeSplit<DecisionResultT>> Finalize();

	private:
		std::unique_ptr<ICondition> m_Condition = nullptr;
		std::unique_ptr<IDecisionTreeNode<DecisionResultT>> m_TrueBranch = nullptr;
		std::unique_ptr<IDecisionTreeNode<DecisionResultT>> m_FalseBranch = nullptr;
	};

	template <typename DecisionResultT>
	class TreeBuilder
	{
	public:
		TreeBuilder<DecisionResultT>& RootNode(std::unique_ptr<IDecisionTreeNode<DecisionResultT>> node);
		std::unique_ptr<DecisionTree<DecisionResultT>> Finalize();

		// Factory functions for tree components (makes sure that all nodes are specialized with the same type)
		std::unique_ptr<SplitBuilder<DecisionResultT>> Split();
		std::unique_ptr<DecisionTreeResult<DecisionResultT>> Result(const DecisionResultT& resultValue);

	private:
		std::unique_ptr<IDecisionTreeNode<DecisionResultT>> m_RootNode;
	};



	template<typename DecisionResultT>
	inline SplitBuilder<DecisionResultT>& SplitBuilder<DecisionResultT>::Condition(std::unique_ptr<ICondition> condition)
	{
		m_Condition = std::move(condition);

		return *this;
	}
	template<typename DecisionResultT>
	inline SplitBuilder<DecisionResultT>& SplitBuilder<DecisionResultT>::True(std::unique_ptr<IDecisionTreeNode<DecisionResultT>> branch)
	{
		m_TrueBranch = std::move(branch);

		return *this;
	}
	template<typename DecisionResultT>
	inline SplitBuilder<DecisionResultT>& SplitBuilder<DecisionResultT>::False(std::unique_ptr<IDecisionTreeNode<DecisionResultT>> branch)
	{
		m_FalseBranch = std::move(branch);

		return *this;
	}
	template<typename DecisionResultT>
	inline std::unique_ptr<DecisionTreeSplit<DecisionResultT>> SplitBuilder<DecisionResultT>::Finalize()
	{
		return std::make_unique<DecisionTreeSplit<DecisionResultT>>(std::move(m_Condition), std::move(m_TrueBranch), std::move(m_FalseBranch));
	}



	template<typename DecisionResultT>
	inline TreeBuilder<DecisionResultT>& TreeBuilder<DecisionResultT>::RootNode(std::unique_ptr<IDecisionTreeNode<DecisionResultT>> node)
	{
		m_RootNode = std::move(node);

		return *this;
	}
	template<typename DecisionResultT>
	inline std::unique_ptr<DecisionTree<DecisionResultT>> TreeBuilder<DecisionResultT>::Finalize()
	{
		return std::make_unique<DecisionTree<DecisionResultT>>(std::move(m_RootNode));
	}
	template<typename DecisionResultT>
	inline std::unique_ptr<SplitBuilder<DecisionResultT>> TreeBuilder<DecisionResultT>::Split()
	{
		return std::make_unique<SplitBuilder<DecisionResultT>>();
	}
	template<typename DecisionResultT>
	inline std::unique_ptr<DecisionTreeResult<DecisionResultT>> TreeBuilder<DecisionResultT>::Result(const DecisionResultT& resultValue)
	{
		return std::make_unique<DecisionTreeResult<DecisionResultT>>(resultValue);
	}
} // namespace dt