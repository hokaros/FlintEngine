#include "SelectedGameObjectManager.h"

void SelectedGameObjectManager::SelectObject(std::shared_ptr<EditorUniversalHandle> game_object)
{
	m_SelectedObject = game_object;

	NotifyGameObjectSelected();
}

std::weak_ptr<EditorUniversalHandle> SelectedGameObjectManager::GetSelectedObject() const
{
	if (m_SelectedObject == nullptr)
		return std::weak_ptr<EditorUniversalHandle>();

	return std::weak_ptr<EditorUniversalHandle>(m_SelectedObject);
}

bool SelectedGameObjectManager::IsObjectSelected(const EditorUniversalHandle& game_object) const
{
	if (m_SelectedObject == nullptr)
		return false;

	return game_object == *m_SelectedObject;
}

void SelectedGameObjectManager::SubscribeSelection(ISelectionObserver& subscriber)
{
	m_SelectionSubscribers.push_back(&subscriber);
}

void SelectedGameObjectManager::OnObjectDestroying(EditorUniversalHandle& game_object) // TODO: IEditable
{
	if (IsObjectSelected(game_object))
	{
		SelectObject(nullptr);
	}
}

void SelectedGameObjectManager::NotifyGameObjectSelected() const
{
	for (ISelectionObserver* subscriber : m_SelectionSubscribers)
	{
		subscriber->OnObjectSelected(GetSelectedObject());
	}
}
