#pragma once
#include <ComponentDefinition.h>
#include <InputController.h>
#include "PlayerEquipment.h"
#include "ConstantMover.h"
#include "Firearm.h"

#define WPN_SWITCH_KEY SDLK_q
#define SHOOT_KEY SDLK_SPACE

class PlayerController :
	public ObjectComponent
{
	DECLARE_COMPONENT();
public:
	// Zdarzenie zmiany kierunku ruchu (nowy kierunek podawany jako argument)
	function<void(Vector)> onMovementChanged;
	function<void(double)> onAimChanged;
	function<void()> onShot;
	function<void(FirearmType)> onWeaponChanged;

public:
	PlayerController() = default;

	void Update() override;
	void Start() override;

protected:
	const IInputController* input = nullptr;
	PlayerEquipment* equipment = nullptr;
	ConstantMover* mover = nullptr;

private:
	void ProcessMovement();
	void ProcessAim();
};

