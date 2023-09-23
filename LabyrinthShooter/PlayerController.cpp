#include "PlayerController.h"
#include "Health.h"

PlayerController::PlayerController(GameObject& player)
	: ObjectComponent(player) {

}

void PlayerController::Start() {
	// Za³adowanie cache'a
	input = InputController::Main();
	equipment = gameObject.FindComponent<PlayerEquipment>();
	mover = gameObject.FindComponent<ConstantMover>();
}

void PlayerController::Update() {
	ProcessMovement();
	ProcessAim();

	// Zmiana broni
	if (input->PressedThisFrame(WPN_SWITCH_KEY)) {
		equipment->SwitchWeapon();

		if (onWeaponChanged)
			onWeaponChanged(equipment->GetCurrentWeapon()->GetType());
	}

	// Strzelanie
	if (input->IsKeyDown(SHOOT_KEY) && equipment->GetCurrentWeapon() != NULL) {
		bool success = equipment->GetCurrentWeapon()->TryShoot();

		if (success && onShot)
			onShot();
	}
}

ObjectComponent* PlayerController::Copy(GameObject& newOwner) {
	return new PlayerController(newOwner);
}

void PlayerController::ProcessMovement() {
	// Odczytanie wciœniêtych klawiszy ruchu
	Vector moveDir;
	if (input->IsKeyDown(SDLK_UP) || input->IsKeyDown(SDLK_w)) {
		moveDir.y -= 1;
	}
	if (input->IsKeyDown(SDLK_DOWN) || input->IsKeyDown(SDLK_s)) {
		moveDir.y += 1;
	}
	if (input->IsKeyDown(SDLK_LEFT) || input->IsKeyDown(SDLK_a)) {
		moveDir.x -= 1;
	}
	if (input->IsKeyDown(SDLK_RIGHT) || input->IsKeyDown(SDLK_d)) {
		moveDir.x += 1;
	}

	if (!mover->IsSameDirection(moveDir)) {
		// Zmieniony kierunek ruchu
		mover->SetDirection(moveDir);

		if (onMovementChanged)
			onMovementChanged(moveDir);
	}
}

void PlayerController::ProcessAim() {
	double prevRotation = gameObject.GetRotation();

	Vector mousePos = input->GetMousePosition();
	gameObject.LookAt(mousePos);

	if (gameObject.GetRotation() != prevRotation && onAimChanged)
		onAimChanged(gameObject.GetRotation());
}