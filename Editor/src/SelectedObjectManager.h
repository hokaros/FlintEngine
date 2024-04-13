#pragma once
#include <ObjectManager.h>
#include <Scene.h>

#include "AssetHandles.h"

class ISelectionObserver
{
public:
	virtual void OnObjectSelected(std::weak_ptr<EditorUniversalHandle> game_object) = 0;

	virtual ~ISelectionObserver() = default;
};

class SelectedObjectManager
{
public:
	void SelectObject(std::shared_ptr<EditorUniversalHandle> game_object);
	std::weak_ptr<EditorUniversalHandle> GetSelectedObject() const;
	bool IsObjectSelected(const EditorUniversalHandle& object) const;

	void SubscribeSelection(ISelectionObserver& subscriber);

	void OnObjectDestroying(EditorUniversalHandle& object);

private:
	void NotifyGameObjectSelected() const;

private:
	std::vector<ISelectionObserver*> m_SelectionSubscribers;

	std::shared_ptr<EditorUniversalHandle> m_SelectedObject;
};

