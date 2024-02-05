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
	auto should_dispose = [this](const GameObject* go) -> bool
	{
		for (GameObject* destroyedGO : m_DestroyedObjects)
		{
			if (destroyedGO == go)
			{
				return true;
			}
		}
		return false; // Not found in destroyed objects
	};

	m_NewObjects.remove_if(should_dispose);
	m_AllObjects.remove_if(should_dispose);
	m_OwnedObjects.remove_if([&should_dispose](const std::unique_ptr<GameObject>& ptr) 
		{
			return should_dispose(ptr.get()); 
		});

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