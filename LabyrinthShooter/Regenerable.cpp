#include "Regenerable.h"

Regenerable::Regenerable(GameObject& owner, double regenerationTime)
	: ObjectComponent(owner), regenerationTime(regenerationTime) {
	gameObject.SubscribeDestroyed([this](GameObject*) {OnDestroyed(); });
}

void Regenerable::Update() {

}

void Regenerable::OnDestroyed() {
	if (!gameObject.IsDestroyed())
		return; // tylko zniszczenie

	Timer::Main()->InvokeOnNextFrame([this]() {
		gameObject.SetDestroyed(false);
		}, regenerationTime);
}

ObjectComponent* Regenerable::Copy(GameObject& newOwner) {
	return new Regenerable(newOwner, regenerationTime);
}