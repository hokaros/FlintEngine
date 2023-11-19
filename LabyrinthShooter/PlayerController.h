#pragma once
#include "../FlatEngine/ObjectComponent.h"
#include "../FlatEngine/InputController.h"
#include "PlayerEquipment.h"
#include "ConstantMover.h"
#include "Firearm.h"

#define WPN_SWITCH_KEY SDLK_q
#define SHOOT_KEY SDLK_SPACE

class PlayerController :
	public ObjectComponent
{
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

	virtual std::unique_ptr<ObjectComponent> Copy() override;
protected:
	InputController* input = NULL;
	PlayerEquipment* equipment = NULL;
	ConstantMover* mover = NULL;

private:
	void ProcessMovement();
	void ProcessAim();
};

