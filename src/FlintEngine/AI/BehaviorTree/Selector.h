#pragma once
#include "Composite.h"

namespace bt
{
	template<typename ContextT>
	class Selector
		: public Composite<ContextT>
	{
	protected:
		virtual void Init() override;
		virtual ENodeStatus Update(ContextT& context) override;

	private:
		typename ChildrenT::iterator m_CurrentChild;
	};



	template<typename ContextT>
	inline void Selector<ContextT>::Init()
	{
		m_CurrentChild = GetChildren().begin();
	}

	template<typename ContextT>
	inline ENodeStatus Selector<ContextT>::Update(ContextT& context)
	{
		while (true)
		{
			if (m_CurrentChild == GetChildren().end())
				return ENodeStatus::Failure; // No success found

			ENodeStatus status = m_CurrentChild->get()->Run(context);

			if (status != ENodeStatus::Failure)
				return status; // We run all nodes in 1 update only if they are all failures

			m_CurrentChild++;
		}
	}
}