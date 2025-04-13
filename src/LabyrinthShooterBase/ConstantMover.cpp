#include "ConstantMover.h"
#include <Core/GameObject.h>

DEFINE_COMPONENT(ConstantMover);
DEFINE_FIELD(ConstantMover, movementSpeed);

ConstantMover::ConstantMover(float movementSpeed)
	: movementSpeed(movementSpeed) 
{

}

void ConstantMover::Update() 
{
	// Ruch
	Vector dPos = moveDir * movementSpeed * Timer::Main()->GetDeltaTime();
	m_GameObject->SetLocalPosition(m_GameObject->GetLocalPosition() + dPos);
}

void ConstantMover::SetDirection(const Vector& newDir) 
{
	std::lock_guard<std::mutex> lock(mutex);

	moveDir = newDir;

	moveDir.Normalize();

	if (onDirectionChanged)
		onDirectionChanged(moveDir);
}

const Vector& ConstantMover::GetDirection() 
{
	std::lock_guard<std::mutex> lock(mutex);
	return moveDir;
}

bool ConstantMover::IsSameDirection(const Vector& otherDir) const 
{
	bool isAnyZero = (otherDir.x == 0 && otherDir.y == 0) || (moveDir.x == 0 && moveDir.y == 0);
	bool areBothZero = otherDir.x == 0 && otherDir.y == 0 && moveDir.x == 0 && moveDir.y == 0;
	if (isAnyZero && !areBothZero)
		return false; // dok³adnie jeden wektor jest zerowy

	return abs(Vector::GetAngle(moveDir, otherDir)) <= DIRECTION_TOLERANCE;
}

void ConstantMover::MoveTowards(const Vector& targetPos)
{
	const Vector currPos = GetPosition();
	const Vector dir = targetPos - currPos;
	SetDirection(dir);
}

void ConstantMover::Stop()
{
	SetDirection(Vector::ZERO);
}

Vector ConstantMover::GetPosition() const
{
	return GetOwner().GetWorldPosition();
}
