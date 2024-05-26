#include "Selector.h"

namespace bt
{
	void Selector::Init()
	{
		m_CurrentChild = GetChildren().begin();
	}

	ENodeStatus Selector::Update()
	{
		while (true)
		{
			if (m_CurrentChild == GetChildren().end())
				return ENodeStatus::Failure; // No success found

			ENodeStatus status = m_CurrentChild->get()->Run();

			if (status != ENodeStatus::Failure)
				return status; // We run all nodes in 1 update only if they are all failures

			m_CurrentChild++;
		}
	}
}