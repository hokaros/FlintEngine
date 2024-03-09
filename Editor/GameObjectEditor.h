#pragma once
#include "../FlatEngine/GameObject.h"
#include "ComponentEditor.h"
#include "AssetHandles.h"
#include "SelectedGameObjectManager.h"
#include "IEditableGameObject.h"

class GameObjectEditor
	: protected IComponentActionObserver
	, protected IGameObjectSelectionObserver
{
public:
	void Init(SelectedGameObjectManager& selected_game_object_manager);

	void Render();

protected:
	// IGameObjectSelectionObserver
	virtual void OnGameObjectSelected(EditorGameObjectHandle* game_object) override;

	// IComponentActionObserver
	virtual void OnComponentDeleted(size_t index_in_game_object) override;

private:
	void LoadAddableComponents();

	void RenderGameObjectEditor(IEditableGameObject& game_object);
	void RenderComponentEditors();
	void RenderComponentAddSection();
	void LoadComponents(IEditableGameObject& game_object);
	void AddComponent(const ComponentDefinition* component);

	void InitValuesFromGameObject(const GameObject& game_object);
	void ApplyValuesToGameObject(IEditableGameObject& game_object);

private:
	static constexpr size_t s_NameMaxSize = 64;

	EditorGameObjectHandle* m_GameObjectHandle = nullptr;
	std::vector<std::unique_ptr<ComponentEditor>> m_ComponentEditors;

	std::vector<const char*> m_AddableComponentNames;
	std::vector<const ComponentDefinition*> m_AddableComponents;

	// TODO: extract GameObjectParamsEditor
	float m_GameObjectPosition[2];
	float m_GameObjectSize[2];
	char m_GameObjectName[s_NameMaxSize];

	bool m_AreComponentEditorsValid = false;

	static constexpr float s_SlidersSpeed = 0.1f;
};

