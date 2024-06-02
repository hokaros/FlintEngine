#pragma once
#include "Composite.h"

namespace bt
{
	template<typename ContextT>
	class Sequence
		: public Composite<ContextT>
	{
	protected:
		virtual void Init() override;
		virtual ENodeStatus Update(ContextT& context) override;

	private:
		typename ChildrenT::iterator m_CurrentChild;
	};



	template<typename ContextT>
	inline void Sequence<ContextT>::Init()
	{
		m_CurrentChild = GetChildren().begin();
	}

	template<typename ContextT>
	inline ENodeStatus Sequence<ContextT>::Update(ContextT& context)
	{
		while (true)
		{
			if (m_CurrentChild == GetChildren().end())
				return ENodeStatus::Success; // Everything was a success (great success)

			ENodeStatus status = m_CurrentChild->get()->Run(context);

			if (status != ENodeStatus::Success)
				return status; // We run all nodes in 1 update only if they are all successes

			m_CurrentChild++;
		}
	}
}