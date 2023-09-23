#include "ConstantMover.h"

ConstantMover::ConstantMover(GameObject& owner, float movementSpeed)
	: ObjectComponent(owner), movementSpeed(movementSpeed) {

}

ObjectComponent* ConstantMover::Copy(GameObject& newOwner) {
	return new ConstantMover(newOwner, movementSpeed);
}

void ConstantMover::Update() {
	// Ruch
	Vector dPos = moveDir * movementSpeed * Timer::Main()->GetDeltaTime();
	gameObject.SetPosition(gameObject.GetPosition() + dPos);
}

void ConstantMover::SetDirection(const Vector& newDir) {
	std::lock_guard<std::mutex> lock(mutex);

	moveDir = newDir;

	moveDir.Normalize();

	if (onDirectionChanged)
		onDirectionChanged(moveDir);
}

const Vector& ConstantMover::GetDirection() {
	std::lock_guard<std::mutex> lock(mutex);
	return moveDir;
}

bool ConstantMover::IsSameDirection(const Vector& otherDir) const {
	bool isAnyZero = (otherDir.x == 0 && otherDir.y == 0) || (moveDir.x == 0 && moveDir.y == 0);
	bool areBothZero = otherDir.x == 0 && otherDir.y == 0 && moveDir.x == 0 && moveDir.y == 0;
	if (isAnyZero && !areBothZero)
		return false; // dok³adnie jeden wektor jest zerowy

	return abs(Vector::GetAngle(moveDir, otherDir)) <= DIRECTION_TOLERANCE;
}