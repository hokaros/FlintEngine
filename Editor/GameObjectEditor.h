#pragma once
#include "../FlatEngine/GameObject.h"
#include "ComponentEditor.h"

class EditorGameObjectHandle
{
public:
	EditorGameObjectHandle(GameObject* game_object);

	GameObject* GetGameObject() const;
	virtual void SaveGameObject();

protected:
	GameObject* m_GameObject;
};

class GameObjectEditor
{
public:
	void Render();
	void SetGameObject(std::unique_ptr<EditorGameObjectHandle> game_object);

private:
	void RenderGameObjectEditor(GameObject& game_object);
	void RenderComponentEditors();
	void LoadComponents(GameObject& game_object);

	void InitValuesFromGameObject(const GameObject& game_object);
	void ApplyValuesToGameObject(GameObject& game_object);

private:
	std::unique_ptr<EditorGameObjectHandle> m_GameObjectHandle = nullptr;
	std::vector<std::unique_ptr<ComponentEditor>> m_ComponentEditors;

	float m_GameObjectPosition[2];
	float m_GameObjectSize[2];

	static constexpr float s_SlidersSpeed = 0.1f;
};

