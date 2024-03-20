#include "SelectedObjectManager.h"

void SelectedObjectManager::SelectObject(std::shared_ptr<EditorUniversalHandle> game_object)
{
	m_SelectedObject = game_object;

	NotifyGameObjectSelected();
}

std::weak_ptr<EditorUniversalHandle> SelectedObjectManager::GetSelectedObject() const
{
	if (m_SelectedObject == nullptr)
		return std::weak_ptr<EditorUniversalHandle>();

	return std::weak_ptr<EditorUniversalHandle>(m_SelectedObject);
}

bool SelectedObjectManager::IsObjectSelected(const EditorUniversalHandle& game_object) const
{
	if (m_SelectedObject == nullptr)
		return false;

	return game_object == *m_SelectedObject;
}

void SelectedObjectManager::SubscribeSelection(ISelectionObserver& subscriber)
{
	m_SelectionSubscribers.push_back(&subscriber);
}

void SelectedObjectManager::OnObjectDestroying(EditorUniversalHandle& game_object) // TODO: IEditable
{
	if (IsObjectSelected(game_object))
	{
		SelectObject(nullptr);
	}
}

void SelectedObjectManager::NotifyGameObjectSelected() const
{
	for (ISelectionObserver* subscriber : m_SelectionSubscribers)
	{
		subscriber->OnObjectSelected(GetSelectedObject());
	}
}
