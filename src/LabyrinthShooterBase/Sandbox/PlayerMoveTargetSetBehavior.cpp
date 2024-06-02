#include "PlayerMoveTargetSetBehavior.h"
#include <optional>


bt::ENodeStatus PlayerMoveTargetSetBehavior::Update(AIContext& context)
{
	std::optional<Vector> player_pos = context.GetPerception().GetPlayerPosition();
	if (!player_pos.has_value())
		return bt::ENodeStatus::Failure;

	context.GetBTNodeMediator().SetMoveTarget(player_pos.value());

	return bt::ENodeStatus::Success;
}
