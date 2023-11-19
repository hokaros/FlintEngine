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
	void RenderStartingWindow();
	void RenderAnotherWindow();

	std::unique_ptr<EditorPrefabHandle> OpenPrefab(const std::string& prefab_path);

private:
	GameObjectEditor m_GameObjectEditor;

	bool m_ShowDemoWindow = false;
	bool m_ShowAnotherWindow = false;
	ImVec4& m_ClearColor;
};

