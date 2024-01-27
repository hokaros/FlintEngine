#pragma once
#include "../FlatEngine/imgui/imgui.h"
#include "GameObjectEditor.h"
#include "AssetExplorer.h"

class Editor
	: private IAssetListener
{
public:
	Editor(ImVec4& clear_color);
	void Render();

private:
	virtual void OnPrefabOpened(std::unique_ptr<EditorPrefabHandle> prefab) override;

	void RenderEditorConfigWindow();

private:
	GameObjectEditor m_GameObjectEditor;
	AssetExplorer m_AssetExplorer;

	ImVec4& m_ClearColor;
};

