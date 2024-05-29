#include "SetRandomMoveTargetBehavior.h"

SetRandomMoveTargetBehavior::SetRandomMoveTargetBehavior(AIContext& context)
	: m_Context(context)
	, m_LastTarget(0)
{
	m_Offsets.push_back(Vector(-50, 0));
	m_Offsets.push_back(Vector(50, 0));
}

bt::ENodeStatus SetRandomMoveTargetBehavior::Update()
{
	const Vector& offset = ChooseOffset();
	const Vector& self_pos = m_Context.GetPerception().GetOwnPosition();

	m_Context.GetBTNodeMediator().SetMoveTarget(self_pos + offset);

	return bt::ENodeStatus::Success;
}

const Vector& SetRandomMoveTargetBehavior::ChooseOffset()
{
	FE_ASSERT(m_Offsets.size() > 0, "No offsets set");

	m_LastTarget++;
	if (m_LastTarget >= m_Offsets.size())
	{
		m_LastTarget = 0;
	}

	return m_Offsets[m_LastTarget];
}
