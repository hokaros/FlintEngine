#include "ObjectManager.h"

ObjectManager* ObjectManager::main{ NULL };

ObjectManager* ObjectManager::Main() {
	return main;
}

ObjectManager::ObjectManager() {
	if (main == NULL) {
		main = this;
	}
}

ObjectManager::~ObjectManager() {
	for (GameObject* go : destroyables) {
		delete go;
	}

	if (main == this) {
		main = NULL;
	}
}

void ObjectManager::AddObject(GameObject* gameObject) {
	allObjects.push_back(gameObject);

	destroyables.push_back(gameObject);

	// Rekursywne dodanie dzieci
	for (GameObject* child : gameObject->GetChildren()) {
		AddObject(child);
	}
}

void ObjectManager::AddUndestroyable(GameObject* gameObject) {
	allObjects.push_back(gameObject);
}

void ObjectManager::DestroyObject(GameObject* gameObject, bool detach) {
	for (GameObject* destroyedObj : destroyed) {
		if (destroyedObj == gameObject)
			return;  // ju¿ usuniêty
	}

	destroyed.push_back(gameObject); // zakolejkowanie do usuniêcia
	gameObject->SetEnabled(false);

	// Od³¹czenie od rodzica
	if (detach && gameObject->GetParent() != NULL) {
		gameObject->GetParent()->RemoveChild(gameObject);
	}

	// Usuniêcie rekurencyjne dzieci
	for (GameObject* child : gameObject->GetChildren()) {
		DestroyObject(child, false);
	}
}

void ObjectManager::DisposeDestroyed() {
	for (GameObject* go : destroyed) {
		delete go;
		destroyables.remove(go);
		allObjects.remove(go);
	}

	destroyed.clear();
}

const std::list<GameObject*>& ObjectManager::GetAllObjects() const {
	return allObjects;
}

void ObjectManager::Clear() {
	for (GameObject* go : destroyables) {
		delete go;
	}

	allObjects.clear();
	destroyables.clear();
	destroyed.clear();
}