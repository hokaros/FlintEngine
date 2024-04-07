#pragma once
#include "../FlatEngine/SceneRenderer.h"
#include "../FlatEngine/imgui/imgui.h"
#include "../FlatEngine/Scene.h"
#include "AssetHandles.h"

class EditorGameRunner
{
public:
	void SetSelectedScene(std::shared_ptr<EditorUniversalHandle> handle);
	void Render();

	void StartGame();
	void StopGame();

private:
	void RenderControlPanel();

	EditorSceneHandle* GetSelectedScene();

private:
	std::shared_ptr<EditorUniversalHandle> m_SelectedSceneHandle = nullptr;
	bool m_IsGameRunning = false;
};

