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
		DecisionTreeSplit(
			ICondition& condition,
			IDecisionTreeNode<ResultT>& trueBranch,
			IDecisionTreeNode<ResultT>& falseBranch);

		virtual const ResultT& Decide() const override;

	private:
		ICondition& m_Condition;

		IDecisionTreeNode<ResultT>& m_TrueBranch;
		IDecisionTreeNode<ResultT>& m_FalseBranch;
	};



	template<typename ResultT>
	inline DecisionTreeSplit<ResultT>::DecisionTreeSplit(
		ICondition& condition,
		IDecisionTreeNode<ResultT>& trueBranch,
		IDecisionTreeNode<ResultT>& falseBranch
	)
		: m_Condition(condition)
		, m_TrueBranch(trueBranch)
		, m_FalseBranch(falseBranch)
	{

	}

	template<typename ResultT>
	inline const ResultT& DecisionTreeSplit<ResultT>::Decide() const
	{
		const IDecisionTreeNode<ResultT>& chosenBranch = m_Condition.Test() ? m_TrueBranch : m_FalseBranch;

		return chosenBranch.Decide();
	}
}