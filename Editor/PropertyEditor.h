#pragma once
#include "../FlatEngine/GameObject.h"
#include "../FlatEngine/IEditableGameObject.h"
#include "ComponentEditor.h"
#include "AssetHandles.h"
#include "SelectedObjectManager.h"

class PropertyEditor
	: protected IComponentActionObserver
	, protected ISelectionObserver
{
public:
	void Init(SelectedObjectManager& selected_game_object_manager);

	void Render();

protected:
	// ISelectionObserver
	virtual void OnObjectSelected(std::weak_ptr<EditorUniversalHandle> object) override;

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

	EditorGameObjectHandle* GetGameObjectHandle();

private:
	static constexpr size_t s_NameMaxSize = 64;

	std::weak_ptr<EditorUniversalHandle> m_GameObjectHandle; // TODO: rename
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

