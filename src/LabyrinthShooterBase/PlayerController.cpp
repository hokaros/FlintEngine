#include "PlayerController.h"
#include <Core/GameObject.h>

DEFINE_COMPONENT(PlayerController);

void PlayerController::Awake() 
{
	// Za�adowanie cache'a
	input = IInputController::Main();
	equipment = m_GameObject->FindComponent<PlayerEquipment>();
	mover = m_GameObject->FindComponent<ConstantMover>();
}

void PlayerController::Update() 
{
	ProcessMovement();

	Controllers::PlayerControlContext context(*input, *equipment);
	m_ActionsController.Update(context);

	ProcessAim();
}

void PlayerController::ProcessMovement() 
{
	// Odczytanie wci�ni�tych klawiszy ruchu
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
	}
}

void PlayerController::ProcessAim() 
{
	float prevRotation = m_GameObject->GetWorldRotation();

	Vector mousePos = input->GetMousePosition();
	m_GameObject->LookAt(mousePos);
}