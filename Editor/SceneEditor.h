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

