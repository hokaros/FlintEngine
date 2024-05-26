#include "Node.h"

namespace bt
{
	ENodeStatus Node::Run()
	{
		if (m_CurrStatus != ENodeStatus::InProgress)
		{
			Init();
		}

		m_CurrStatus = Update();

		if (m_CurrStatus != ENodeStatus::InProgress)
		{
			Deinit();
		}

		return m_CurrStatus;
	}
}