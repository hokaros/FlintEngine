#pragma once
#include "../FlatEngine/imgui/imgui.h"
#include "../FlatEngine/PrefabLoader.h"
#include "GameObjectEditor.h"
#include "PrefabSaver.h"

class Editor
{
public:
	Editor(ImVec4& clear_color);
	void Render();

private:
	void RenderEditorConfigWindow();

	std::unique_ptr<EditorPrefabHandle> OpenPrefab(const std::string& prefab_path);

private:
	GameObjectEditor m_GameObjectEditor;

	ImVec4& m_ClearColor;
};

