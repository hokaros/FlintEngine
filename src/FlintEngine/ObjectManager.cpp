#include "ObjectManager.h"

#include <ftl.h>

ObjectManager* ObjectManager::s_Main = nullptr;

ObjectManager* ObjectManager::Main() 
{
	return s_Main;
}

void ObjectManager::SetAsMain()
{
	s_Main = this;
}

void ObjectManager::DestroyObjectImpl(GameObject* gameObject, bool detach)
{
	for (GameObject* destroyedObj : m_DestroyedObjects) {
		if (destroyedObj == gameObject)
			return;  // ju¿ usuniêty
	}

	gameObject->GetUpdateable().OnDestroy();

	m_DestroyedObjects.push_back(gameObject); // zakolejkowanie do usuniêcia
	gameObject->SetEnabled(false);

	// Od³¹czenie od rodzica
	if (detach && gameObject->GetParent() != nullptr) 
	{
		gameObject->GetParent()->MoveChild(gameObject, *this);
	}
}

void ObjectManager::NotifyObjectDestroying(GameObject* object)
{
	if (object == nullptr)
		return;

	for (IObjectManagerObserver* observer : m_Observers)
	{
		observer->OnObjectDestroying(*object);
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

const std::vector<std::unique_ptr<GameObject>>& ObjectManager::GetGameObjects() const
{
	return m_OwnedObjects;
}

void ObjectManager::RemoveGameObject(GameObject& game_object)
{
	DestroyObjectImpl(&game_object, true);
	DisposeDestroyed();
}

void ObjectManager::AddToMessageSubscribers(GameObject* gameObject)
{
	m_NewMessageSubscribers.push_back(gameObject);
	m_MessageSubscribers.push_back(gameObject);
}

void ObjectManager::DestroyObject(GameObject* gameObject) 
{
	NotifyObjectDestroying(gameObject);

	DestroyObjectImpl(gameObject, true);
}

void ObjectManager::DestroyAll()
{
	for (std::unique_ptr<GameObject>& go : m_OwnedObjects)
	{
		DestroyObject(go.get());
	}
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
	ftl::vector_remove_if(m_OwnedObjects, [&should_dispose](const std::unique_ptr<GameObject>& ptr) 
	{
		return should_dispose(ptr.get()); 
	});

	m_DestroyedObjects.clear();
}

void ObjectManager::ActivateNewObjects()
{
	for (GameObject* go : m_NewMessageSubscribers)
	{
		go->GetUpdateable().Awake();
	}
	for (GameObject* go : m_NewMessageSubscribers)
	{
		go->GetUpdateable().Start();
	}

	m_NewMessageSubscribers.clear();
}

const std::list<GameObject*>& ObjectManager::GetAllMessageSubscribers() const 
{
	return m_MessageSubscribers;
}

const std::vector<std::unique_ptr<GameObject>>& ObjectManager::GetOwnedObjects() const
{
	return m_OwnedObjects;
}

std::vector<std::unique_ptr<GameObject>>& ObjectManager::GetOwnedObjects()
{
	return m_OwnedObjects;
}

void ObjectManager::Subscribe(IObjectManagerObserver& observer)
{
	m_Observers.push_back(&observer);
}

void ObjectManager::Clear() 
{
	m_MessageSubscribers.clear();
	m_NewMessageSubscribers.clear();
	m_DestroyedObjects.clear();

	m_OwnedObjects.clear();
}