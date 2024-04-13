#include "PlayerController.h"
#include "Health.h"
#include <Core/GameObject.h>

DEFINE_COMPONENT(PlayerController);

void PlayerController::Start() 
{
	// Za³adowanie cache'a
	input = IInputController::Main();
	equipment = m_GameObject->FindComponent<PlayerEquipment>();
	mover = m_GameObject->FindComponent<ConstantMover>();
}

void PlayerController::Update() 
{
	ProcessMovement();
	ProcessAim();

	// Zmiana broni
	if (input->PressedThisFrame(WPN_SWITCH_KEY)) 
	{
		equipment->SwitchWeapon();

		if (onWeaponChanged)
			onWeaponChanged(equipment->GetCurrentWeapon()->GetType());
	}

	// Strzelanie
	if (input->IsKeyDown(SHOOT_KEY) && equipment->GetCurrentWeapon() != NULL) 
	{
		bool success = equipment->GetCurrentWeapon()->TryShoot();

		if (success && onShot)
			onShot();
	}
}

void PlayerController::ProcessMovement() 
{
	// Odczytanie wciœniêtych klawiszy ruchu
	Vector moveDir;
	if (input->IsKeyDown(SDLK_UP) || input->IsKeyDown(SDLK_w)) 
	{
		moveDir.y -= 1;
	}
	if (input->IsKeyDown(SDLK_DOWN) || input->IsKeyDown(SDLK_s)) 
	{
		moveDir.y += 1;
	}
	if (input->IsKeyDown(SDLK_LEFT) || input->IsKeyDown(SDLK_a)) 
	{
		moveDir.x -= 1;
	}
	if (input->IsKeyDown(SDLK_RIGHT) || input->IsKeyDown(SDLK_d)) 
	{
		moveDir.x += 1;
	}

	if (!mover->IsSameDirection(moveDir)) 
	{
		// Zmieniony kierunek ruchu
		mover->SetDirection(moveDir);

		if (onMovementChanged)
			onMovementChanged(moveDir);
	}
}

void PlayerController::ProcessAim() 
{
	double prevRotation = m_GameObject->GetRotation();

	Vector mousePos = input->GetMousePosition();
	m_GameObject->LookAt(mousePos);

	if (m_GameObject->GetRotation() != prevRotation && onAimChanged)
		onAimChanged(m_GameObject->GetRotation());
}