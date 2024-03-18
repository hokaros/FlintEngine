#pragma once
#include "../FlatEngine/imgui/imgui.h"
#include "../FlatEngine/AssetManager.h"
#include "GameObjectEditor.h"
#include "AssetExplorer.h"
#include "SceneEditor.h"
#include "HierarchyEditor.h"
#include "SelectedGameObjectManager.h"

class Editor
	: private IAssetListener
{
public:
	Editor(ImVec4& clear_color, SDL_Renderer& renderer, int screenWidth, int screenHeight);
	void Render();

private:
	virtual void OnPrefabOpened(std::unique_ptr<EditorPrefabHandle> prefab) override;
	virtual void OnSceneOpened(std::unique_ptr<EditorSceneHandle> scene) override;

	void RenderEditorConfigWindow();

private:
	GameObjectEditor m_GameObjectEditor;
	AssetExplorer m_AssetExplorer;
	AssetManager m_AssetManager;
	SceneEditor m_SceneEditor;
	HierarchyEditor m_HierarchyEditor;

	SelectedGameObjectManager m_SelectedGameObjectManager;

	ImVec4& m_ClearColor;
};

