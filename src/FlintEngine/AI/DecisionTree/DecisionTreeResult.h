#pragma once

#include <AI/DecisionTree/IDecisionTreeNode.h>

#include <utility>

namespace dt
{
	template <typename ResultT>
	class DecisionTreeResult
		: public IDecisionTreeNode<ResultT>
	{
	public:
		DecisionTreeResult(const ResultT& result);

		virtual const ResultT& Decide() const override;

	private:
		ResultT m_Result;
	};


	template<typename ResultT>
	inline DecisionTreeResult<ResultT>::DecisionTreeResult(const ResultT& result)
		: m_Result(result)
	{
	}
	template<typename ResultT>
	inline const ResultT& DecisionTreeResult<ResultT>::Decide() const
	{
		return m_Result;
	}
}