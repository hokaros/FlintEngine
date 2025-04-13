#include "PlayerActionsController.h"

#include <PlayerEquipment.h>

namespace Controllers
{
	void PlayerActionsController::Update(PlayerControlContext& context)
	{
		const IInputController& input = context.GetInput();
		PlayerEquipment& equipment = context.GetEquipment();

		// Zmiana broni
		if (input.PressedThisFrame(WPN_SWITCH_KEY))
		{
			equipment.SwitchWeapon();
		}

		// Strzelanie
		if (input.IsKeyDown(SHOOT_KEY))
		{
			if (Firearm* currWeapon = equipment.GetCurrentWeapon())
			{
				currWeapon->TryShoot();
			}
		}
	}
}
