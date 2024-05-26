#include "PlayerMoveTargetSetBehavior.h"
#include <optional>


PlayerMoveTargetSetBehavior::PlayerMoveTargetSetBehavior(AIContext& context)
	: m_Context(context)
{
}

bt::ENodeStatus PlayerMoveTargetSetBehavior::Update()
{
	std::optional<Vector> player_pos = m_Context.GetPerception().GetPlayerPosition();
	if (!player_pos.has_value())
		return bt::ENodeStatus::Failure;

	m_Context.GetBTNodeMediator().SetMoveTarget(player_pos.value());

	return bt::ENodeStatus::Success;
}
