#pragma once
#include <imgui/imgui.h>
#include <AssetManager.h>
#include <PropertyEditor/PropertyEditor.h>
#include <AssetExplorer/AssetExplorer.h>
#include "SceneEditor.h"
#include "HierarchyEditor.h"
#include "SelectedObjectManager.h"
#include "EditorGameRunner.h"

class Editor
	: private IAssetListener
{
public:
	Editor(ImVec4& clear_color, SDL_Renderer& renderer, int screenWidth, int screenHeight);
	void Render();

	EditorGameRunner& GetGameRunner();

private:
	virtual void OnPrefabOpened(std::unique_ptr<EditorPrefabHandle> prefab) override;
	virtual void OnSceneOpened(std::unique_ptr<EditorSceneHandle> scene) override;

	void BeginEditorSpace();
	void EndEditorSpace();
	void RenderEditorConfigWindow();

private:
	PropertyEditor m_PropertyEditor;
	AssetExplorer m_AssetExplorer;
	AssetManager m_AssetManager;
	SceneEditor m_SceneEditor;
	HierarchyEditor m_HierarchyEditor;
	EditorGameRunner m_GameRunner;

	SelectedObjectManager m_SelectedGameObjectManager;

	ImVec4& m_ClearColor;
};

