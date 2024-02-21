#include "SelectedGameObjectManager.h"

void SelectedGameObjectManager::OnSceneLoaded(Scene& new_scene)
{
	new_scene.GetObjectManager().Subscribe(*this);
}

void SelectedGameObjectManager::SelectGameObject(std::shared_ptr<EditorGameObjectHandle> game_object)
{
	m_SelectedGameObject = game_object;

	NotifyGameObjectSelected();
}

EditorGameObjectHandle* SelectedGameObjectManager::GetSelectedGameObject() const
{
	return m_SelectedGameObject.get();
}

bool SelectedGameObjectManager::IsGameObjectSelected(const GameObject& game_object) const
{
	if (m_SelectedGameObject == nullptr)
		return false;

	return &game_object == m_SelectedGameObject->GetGameObject();
}

void SelectedGameObjectManager::SubscribeSelection(IGameObjectSelectionObserver& subscriber)
{
	m_SelectionSubscribers.push_back(&subscriber);
}

void SelectedGameObjectManager::OnObjectDestroying(GameObject& game_object)
{
	if (m_SelectedGameObject == nullptr)
		return;

	if (m_SelectedGameObject->GetGameObject() == &game_object)
	{
		SelectGameObject(nullptr);
	}
}

void SelectedGameObjectManager::NotifyGameObjectSelected() const
{
	for (IGameObjectSelectionObserver* subscriber : m_SelectionSubscribers)
	{
		subscriber->OnGameObjectSelected(m_SelectedGameObject.get());
	}
}
