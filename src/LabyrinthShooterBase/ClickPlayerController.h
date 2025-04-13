#pragma once
#include <ComponentDefinition.h>
#include <InputController.h>
#include <Controllers/PlayerActionsController.h>
#include <Navigation/NavmeshAgent.h>
#include "PlayerEquipment.h"
#include "ConstantMover.h"
#include "Firearm.h"

class ClickPlayerController
	: public ObjectComponent
{
	DECLARE_COMPONENT();
public:
	virtual void Awake() override;
	virtual void Update() override;

protected:
	const IInputController* input = nullptr;
	PlayerEquipment* equipment = nullptr;
	ConstantMover* mover = nullptr;

private:
	void ProcessMovement();

private:
	Controllers::PlayerActionsController m_ActionsController;
	std::unique_ptr<Navigation::NavmeshAgent> m_NavmeshAgent = nullptr;
};