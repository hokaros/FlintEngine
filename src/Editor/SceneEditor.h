#pragma once
#include <SceneRenderer.h>
#include <imgui/imgui.h>
#include <Core/Scene.h>
#include <AssetHandles/EditorUniversalHandle.h>
#include "ViewportController.h"
#include "SelectedObjectManager.h"

class SceneEditor
{
public:
	SceneEditor(SDL_Renderer& renderer, float screenWidth, float screenHeight);

	void Init(SelectedObjectManager& selected_object_manager);

	void SetRootObject(std::weak_ptr<EditorUniversalHandle> root_object); // TODO: make this for scenes also

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

	EditableScene* m_CurrentScene;

	EditableScene m_PrefabScene;

	ViewportController viewportController;

	std::weak_ptr<EditorUniversalHandle> m_RootObject;

	SelectedObjectManager* m_SelectedObjectManager;
};

