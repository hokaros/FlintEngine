#include "SelectedGameObjectManager.h"

void SelectedGameObjectManager::OnSceneLoaded(Scene& new_scene)
{
	new_scene.GetObjectManager().Subscribe(*this);
}

void SelectedGameObjectManager::SelectGameObject(std::shared_ptr<EditorUniversalHandle> game_object)
{
	FE_ASSERT(game_object == nullptr || game_object->GetGameObjectHandle() != nullptr, "No GameObjectHandle passed");

	m_SelectedGameObject = game_object;

	NotifyGameObjectSelected();
}

EditorGameObjectHandle* SelectedGameObjectManager::GetSelectedGameObject() const
{
	if (m_SelectedGameObject == nullptr)
		return nullptr;

	return m_SelectedGameObject->GetGameObjectHandle().get();
}

bool SelectedGameObjectManager::IsGameObjectSelected(const GameObject& game_object) const
{
	if (m_SelectedGameObject == nullptr || m_SelectedGameObject->GetGameObjectHandle()->GetGameObject() == nullptr)
		return false;

	return &game_object == &m_SelectedGameObject->GetGameObjectHandle()->GetGameObject()->GetResult();
}

void SelectedGameObjectManager::SubscribeSelection(IGameObjectSelectionObserver& subscriber)
{
	m_SelectionSubscribers.push_back(&subscriber);
}

void SelectedGameObjectManager::OnObjectDestroying(GameObject& game_object) // TODO: IEditable
{
	if (m_SelectedGameObject == nullptr || m_SelectedGameObject->GetGameObjectHandle()->GetGameObject() == nullptr)
		return;

	if (&m_SelectedGameObject->GetGameObjectHandle()->GetGameObject()->GetResult() == &game_object)
	{
		SelectGameObject(nullptr);
	}
}

void SelectedGameObjectManager::NotifyGameObjectSelected() const
{
	for (IGameObjectSelectionObserver* subscriber : m_SelectionSubscribers)
	{
		subscriber->OnGameObjectSelected(GetSelectedGameObject());
	}
}
