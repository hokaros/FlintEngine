#pragma once
#include <AI/DecisionTree/IDecisionTreeNode.h>
#include <AI/DecisionTree/ICondition.h>
#include <utility.h>

#include <memory>

namespace dt
{
	template <typename ResultT>
	class DecisionTreeSplit
		: public IDecisionTreeNode<ResultT>
	{
	public:
		DecisionTreeSplit<ResultT>(
			std::unique_ptr<ICondition> condition,
			std::unique_ptr<IDecisionTreeNode<ResultT>> trueBranch,
			std::unique_ptr<IDecisionTreeNode<ResultT>> falseBranch);

		virtual const ResultT& Decide() const override;

	private:
		std::unique_ptr<ICondition> m_Condition;

		std::unique_ptr<IDecisionTreeNode<ResultT>> m_TrueBranch;
		std::unique_ptr<IDecisionTreeNode<ResultT>> m_FalseBranch;
	};



	template<typename ResultT>
	inline DecisionTreeSplit<ResultT>::DecisionTreeSplit(
		std::unique_ptr<ICondition> condition,
		std::unique_ptr<IDecisionTreeNode<ResultT>> trueBranch,
		std::unique_ptr<IDecisionTreeNode<ResultT>> falseBranch
	)
		: m_Condition(std::move(condition))
		, m_TrueBranch(std::move(trueBranch))
		, m_FalseBranch(std::move(falseBranch))
	{
		FE_ASSERT(m_Condition != nullptr && m_TrueBranch != nullptr && m_FalseBranch != nullptr, "Incomplete initialization of DecisionTreeSplit");
	}

	template<typename ResultT>
	inline const ResultT& DecisionTreeSplit<ResultT>::Decide() const
	{
		const IDecisionTreeNode<ResultT>& chosenBranch = m_Condition->Test() ? *m_TrueBranch : *m_FalseBranch;

		return chosenBranch.Decide();
	}
}