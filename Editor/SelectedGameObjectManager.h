#pragma once
#include "AssetHandles.h"

class IGameObjectSelectionObserver
{
public:
	virtual void OnGameObjectSelected(EditorGameObjectHandle* game_object) = 0;

	virtual ~IGameObjectSelectionObserver() = default;
};

class SelectedGameObjectManager
{
public:
	void SelectGameObject(std::shared_ptr<EditorGameObjectHandle> game_object);
	EditorGameObjectHandle* GetSelectedGameObject() const;
	bool IsGameObjectSelected(const GameObject& game_object) const;

	void SubscribeSelection(IGameObjectSelectionObserver& subscriber);

	// Should be called when any GameObject in the editor is destroyed
	void OnGameObjectDestroyed(GameObject& game_object);

private:
	void NotifyGameObjectSelected() const;

private:
	std::vector<IGameObjectSelectionObserver*> m_SelectionSubscribers;

	std::shared_ptr<EditorGameObjectHandle> m_SelectedGameObject;
};

