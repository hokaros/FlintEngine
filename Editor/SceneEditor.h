#pragma once
#include "../FlatEngine/SceneRenderer.h"
#include "../FlatEngine/imgui/imgui.h"
#include "../FlatEngine/Scene.h"
#include "AssetHandles.h"
#include "ViewportController.h"

class SceneEditor
{
public:
	SceneEditor(SDL_Renderer& renderer, float screenWidth, float screenHeight);

	void SetRootObject(std::weak_ptr<EditorUniversalHandle> root_object); // TODO: make this for scenes also

	void Render();

	Scene& GetScene();

private:
	void AddExampleObjectsToScene();
	void ResetRootObject();

	void RenderOverlay();

private:
	SceneRenderer m_SceneRenderer;
	Scene m_Scene; // TODO: transform into Asset Handle

	ViewportController viewportController;

	std::weak_ptr<EditorUniversalHandle> m_RootObject;
};

