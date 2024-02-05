#include "ObjectManager.h"

ObjectManager* ObjectManager::s_Main = nullptr;

ObjectManager* ObjectManager::Main() 
{
	return s_Main;
}

void ObjectManager::DestroyObjectImpl(GameObject* gameObject, bool detach)
{
	for (GameObject* destroyedObj : m_DestroyedObjects) {
		if (destroyedObj == gameObject)
			return;  // ju¿ usuniêty
	}

	gameObject->OnDestroy();

	m_DestroyedObjects.push_back(gameObject); // zakolejkowanie do usuniêcia
	gameObject->SetEnabled(false);

	// Od³¹czenie od rodzica
	if (detach && gameObject->GetParent() != nullptr) 
	{
		gameObject->GetParent()->RemoveChild(gameObject);
	}

	// Usuniêcie rekurencyjne dzieci
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
	if (s_Main == this) 
	{
		s_Main = nullptr;
	}
}

void ObjectManager::AddObject(std::unique_ptr<GameObject> gameObject) 
{
	m_NewObjects.push_back(gameObject.get());
	m_AllObjects.push_back(gameObject.get());

	// Rekursywne dodanie dzieci
	for (GameObject* child : gameObject->GetChildren()) 
	{
		AddObject(std::unique_ptr<GameObject>(child));
	}

	m_OwnedObjects.push_back(std::move(gameObject));
}

void ObjectManager::AddUndestroyable(GameObject* gameObject) 
{
	m_NewObjects.push_back(gameObject);
	m_AllObjects.push_back(gameObject);
}

void ObjectManager::DestroyObject(GameObject* gameObject) 
{
	DestroyObjectImpl(gameObject, true);
}

void ObjectManager::DisposeDestroyed() 
{
	for (GameObject* go : m_DestroyedObjects) 
	{
		m_NewObjects.remove(go);
		m_AllObjects.remove(go);

		m_OwnedObjects.remove_if([go](const std::unique_ptr<GameObject>& ownedGO) -> bool {
			return ownedGO.get() == go;
		});
	}

	m_DestroyedObjects.clear();
}

void ObjectManager::ActivateNewObjects()
{
	for (GameObject* go : m_NewObjects)
	{
		go->Awake();
	}
	for (GameObject* go : m_NewObjects)
	{
		go->Start();
	}

	m_NewObjects.clear();
}

const std::list<GameObject*>& ObjectManager::GetAllObjects() const 
{
	return m_AllObjects;
}

void ObjectManager::Clear() 
{
	m_AllObjects.clear();
	m_DestroyedObjects.clear();

	m_OwnedObjects.clear();
}