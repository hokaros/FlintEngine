#include "ObjectManager.h"

ObjectManager* ObjectManager::s_Main = nullptr;

ObjectManager* ObjectManager::Main() 
{
	return s_Main;
}

void ObjectManager::DestroyObjectImpl(GameObject* gameObject, bool detach)
{
	for (GameObject* destroyedObj : destroyed) {
		if (destroyedObj == gameObject)
			return;  // ju� usuni�ty
	}

	gameObject->OnDestroy();

	destroyed.push_back(gameObject); // zakolejkowanie do usuni�cia
	gameObject->SetEnabled(false);

	// Od��czenie od rodzica
	if (detach && gameObject->GetParent() != nullptr) 
	{
		gameObject->GetParent()->RemoveChild(gameObject);
	}

	// Usuni�cie rekurencyjne dzieci
	for (GameObject* child : gameObject->GetChildren()) 
	{
		DestroyObjectImpl(child, false);
	}
}

ObjectManager::ObjectManager() 
{
	if (s_Main == nullptr) 
	{
		s_Main = this;
	}
}

ObjectManager::~ObjectManager() 
{
	for (GameObject* go : destroyables) 
	{
		delete go;
	}

	if (s_Main == this) 
	{
		s_Main = nullptr;
	}
}

void ObjectManager::AddObject(GameObject* gameObject) 
{
	newObjects.push_back(gameObject);
	allObjects.push_back(gameObject);

	destroyables.push_back(gameObject);

	// Rekursywne dodanie dzieci
	for (GameObject* child : gameObject->GetChildren()) 
	{
		AddObject(child);
	}
}

void ObjectManager::AddUndestroyable(GameObject* gameObject) 
{
	allObjects.push_back(gameObject);
}

void ObjectManager::DestroyObject(GameObject* gameObject) 
{
	DestroyObjectImpl(gameObject, true);
}

void ObjectManager::DisposeDestroyed() 
{
	for (GameObject* go : destroyed) 
	{
		delete go;
		destroyables.remove(go);
		allObjects.remove(go);
	}

	destroyed.clear();
}

void ObjectManager::ActivateNewObjects()
{
	for (GameObject* go : newObjects)
	{
		go->Awake();
	}
	for (GameObject* go : newObjects)
	{
		go->Start();
	}

	newObjects.clear();
}

const std::list<GameObject*>& ObjectManager::GetAllObjects() const 
{
	return allObjects;
}

void ObjectManager::Clear() 
{
	for (GameObject* go : destroyables) 
	{
		delete go;
	}

	allObjects.clear();
	destroyables.clear();
	destroyed.clear();
}