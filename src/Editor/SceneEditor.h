#pragma once
#include <SceneRenderer.h>
#include <imgui/imgui.h>
#include <Scene.h>
#include <AssetHandles/EditorUniversalHandle.h>
#include "ViewportController.h"

class SceneEditor
{
public:
	SceneEditor(SDL_Renderer& renderer, float screenWidth, float screenHeight);

	void SetRootObject(std::weak_ptr<EditorUniversalHandle> root_object); // TODO: make this for scenes also

	void Render();

private:
	void ResetRootObject();

	void RenderOverlay();

	static Rgb8 GetNegativeColor(const Rgb8& color);

private:
	SceneRenderer m_SceneRenderer;

	EditableScene* m_CurrentScene;

	EditableScene m_PrefabScene;

	ViewportController viewportController;

	std::weak_ptr<EditorUniversalHandle> m_RootObject;
};

