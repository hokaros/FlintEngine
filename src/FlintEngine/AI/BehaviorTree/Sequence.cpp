#include "Sequence.h"

namespace bt
{
	void Sequence::Init()
	{
		m_CurrentChild = GetChildren().begin();
	}

	ENodeStatus Sequence::Update()
	{
		while (true)
		{
			if (m_CurrentChild == GetChildren().end())
				return ENodeStatus::Success; // Everything was a success (great success)

			ENodeStatus status = m_CurrentChild->get()->Run();

			if (status != ENodeStatus::Success)
				return status; // We run all nodes in 1 update only if they are all successes

			m_CurrentChild++;
		}
	}
}