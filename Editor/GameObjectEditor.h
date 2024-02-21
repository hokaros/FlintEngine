#pragma once
#include "../FlatEngine/GameObject.h"
#include "ComponentEditor.h"
#include "AssetHandles.h"

class GameObjectEditor
	: protected IComponentActionObserver
{
public:
	void Render();
	void SetGameObject(std::shared_ptr<EditorGameObjectHandle> game_object);

private:
	void LoadAddableComponents();

	void RenderGameObjectEditor(GameObject& game_object);
	void RenderComponentEditors();
	void RenderComponentAddSection();
	void LoadComponents(GameObject& game_object);
	void AddComponent(const ComponentDefinition* component);

	void InitValuesFromGameObject(const GameObject& game_object);
	void ApplyValuesToGameObject(GameObject& game_object);

	// IComponentActionObserver
	virtual void OnComponentDeleted(size_t index_in_game_object) override;

private:
	static constexpr size_t s_NameMaxSize = 64;

	std::shared_ptr<EditorGameObjectHandle> m_GameObjectHandle = nullptr;
	std::vector<std::unique_ptr<ComponentEditor>> m_ComponentEditors;

	std::vector<const char*> m_AddableComponentNames;
	std::vector<const ComponentDefinition*> m_AddableComponents;

	float m_GameObjectPosition[2];
	float m_GameObjectSize[2];
	char m_GameObjectName[s_NameMaxSize];

	bool m_AreComponentEditorsValid = false;

	static constexpr float s_SlidersSpeed = 0.1f;
};

