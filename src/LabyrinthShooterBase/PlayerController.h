#pragma once
#include <ComponentDefinition.h>
#include <InputController.h>
#include "PlayerEquipment.h"
#include "ConstantMover.h"
#include "Firearm.h"
#include <Controllers/PlayerActionsController.h>

class PlayerController :
	public ObjectComponent
{
	DECLARE_COMPONENT();

public:
	void Update() override;
	void Awake() override;

protected:
	const IInputController* input = nullptr;
	PlayerEquipment* equipment = nullptr;
	ConstantMover* mover = nullptr;

private:
	void ProcessMovement();
	void ProcessAim();

private:
	Controllers::PlayerActionsController m_ActionsController;
};

