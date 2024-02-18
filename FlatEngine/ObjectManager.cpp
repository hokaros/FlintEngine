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
		gameObject->GetParent()->MoveChild(gameObject, *this);
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

void ObjectManager::AddNewObject(std::unique_ptr<GameObject> gameObject)
{
	AddToMessageSubscribers(gameObject.get());

	AddGameObject(std::move(gameObject));
}

void ObjectManager::AddGameObject(std::unique_ptr<GameObject> gameObject)
{
	m_OwnedObjects.push_back(std::move(gameObject));
}

void ObjectManager::AddToMessageSubscribers(GameObject* gameObject)
{
	m_NewMessageSubscribers.push_back(gameObject);
	m_MessageSubscribers.push_back(gameObject);
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

	m_NewMessageSubscribers.remove_if(should_dispose);
	m_MessageSubscribers.remove_if(should_dispose);
	m_OwnedObjects.remove_if([&should_dispose](const std::unique_ptr<GameObject>& ptr) 
		{
			return should_dispose(ptr.get()); 
		});

	m_DestroyedObjects.clear();
}

void ObjectManager::ActivateNewObjects()
{
	for (GameObject* go : m_NewMessageSubscribers)
	{
		go->Awake();
	}
	for (GameObject* go : m_NewMessageSubscribers)
	{
		go->Start();
	}

	m_NewMessageSubscribers.clear();
}

const std::list<GameObject*>& ObjectManager::GetAllMessageSubscribers() const 
{
	return m_MessageSubscribers;
}

void ObjectManager::Clear() 
{
	m_MessageSubscribers.clear();
	m_DestroyedObjects.clear();

	m_OwnedObjects.clear();
}