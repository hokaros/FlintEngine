#pragma once
#include "BTEnums.h"

namespace bt
{
	template <typename ContextT>
	class Node
	{
	public:
		ENodeStatus Run(ContextT& context);

	protected:
		virtual void Init() {}
		virtual ENodeStatus Update(ContextT& context) = 0;
		virtual void Deinit() {}

	private:
		// TODO: probably not all nodes need to call Init or Deinit so we don't need to track status for them
		ENodeStatus m_CurrStatus = ENodeStatus::Invalid;
	};



	template<typename ContextT>
	inline ENodeStatus Node<ContextT>::Run(ContextT& context)
	{
		if (m_CurrStatus != ENodeStatus::InProgress)
		{
			Init();
		}

		m_CurrStatus = Update(context);

		if (m_CurrStatus != ENodeStatus::InProgress)
		{
			Deinit();
		}

		return m_CurrStatus;
	}
}