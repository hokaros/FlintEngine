#pragma once
#include <InputController.h>
#include "PlayerControlContext.h"

#define WPN_SWITCH_KEY SDLK_q
#define SHOOT_KEY SDLK_SPACE

namespace Controllers
{
	class PlayerActionsController
	{
	public:
		void Update(PlayerControlContext& context);
	};
}