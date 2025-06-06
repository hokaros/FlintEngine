#pragma once
#include <Rendering/SceneRenderer.h>
#include <imgui/imgui.h>
#include <Scene/Scene.h>
#include <AssetHandles/EditorUniversalHandle.h>
#include "ViewportController.h"
#include "SelectedObjectManager.h"

class SceneEditor
{
public:
	SceneEditor(SDL_Renderer& renderer, float screenWidth, float screenHeight);

	void Init(SelectedObjectManager& selected_object_manager);

	void SetRootObject(std::weak_ptr<EditorUniversalHandle> root_object);

	void Render();

private:
	void ResetRootObject();

	void RenderOverlay();

	void ProcessShortcuts();

	void MoveViewportToSelectedGameObject();
	void MoveViewportToGameObject(const GameObject& object);

	static Rgb8 GetNegativeColor(const Rgb8& color);

private:
	SceneRenderer m_SceneRenderer;

	Scene* m_CurrentScene;

	Scene m_PrefabScene;

	ViewportController viewportController;

	std::weak_ptr<EditorUniversalHandle> m_RootObject;

	SelectedObjectManager* m_SelectedObjectManager;
};

