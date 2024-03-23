#pragma once
#include "ComponentEditor.h"
#include "AssetHandles.h"
#include "IPropertyEditor.h"


class GameObjectPropertyEditor
	: protected IComponentActionObserver
	, public IPropertyEditor
{
public:
	virtual void RenderEmbedded() override;
	virtual bool HasValidObject() const override;

	void SetGameObject(std::weak_ptr<EditorUniversalHandle> game_object); // TODO: let's see if we can pass EditorGameObjectHandle here

protected:
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
	const EditorGameObjectHandle* GetGameObjectHandle() const;

private:
	static constexpr size_t s_NameMaxSize = 64;

	std::weak_ptr<EditorUniversalHandle> m_GameObjectHandle;

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

