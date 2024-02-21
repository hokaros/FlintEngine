#pragma once
#include "../FlatEngine/ObjectManager.h"
#include "../FlatEngine/Scene.h"

#include "AssetHandles.h"

class IGameObjectSelectionObserver
{
public:
	virtual void OnGameObjectSelected(EditorGameObjectHandle* game_object) = 0;

	virtual ~IGameObjectSelectionObserver() = default;
};

class SelectedGameObjectManager
	: public IObjectManagerObserver
{
public:
	void OnSceneLoaded(Scene& new_scene);

	void SelectGameObject(std::shared_ptr<EditorGameObjectHandle> game_object);
	EditorGameObjectHandle* GetSelectedGameObject() const;
	bool IsGameObjectSelected(const GameObject& game_object) const;

	void SubscribeSelection(IGameObjectSelectionObserver& subscriber);

	virtual void OnObjectDestroying(GameObject& game_object) override;

private:
	void NotifyGameObjectSelected() const;

private:
	std::vector<IGameObjectSelectionObserver*> m_SelectionSubscribers;

	std::shared_ptr<EditorGameObjectHandle> m_SelectedGameObject;
};

