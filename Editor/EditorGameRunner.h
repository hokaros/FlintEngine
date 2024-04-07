#pragma once
#include "../FlatEngine/SceneRenderer.h"
#include "../FlatEngine/imgui/imgui.h"
#include "../FlatEngine/Scene.h"
#include "../FlatEngine/InputController.h"
#include "../FlatEngine/GameBase.h"
#include "AssetHandles.h"

class EditorGameRunner
{
public:
	EditorGameRunner(SDL_Renderer& renderer, float screenWidth, float screenHeight);

	void SetSelectedScene(std::shared_ptr<EditorUniversalHandle> handle);
	void Render();

	void StartGame();
	void StopGame();
	bool IsGameRunning() const;

	InputController& GetInputController();

private:
	void RenderControlPanel();
	void RenderGameFrame();
	void ClearGameFrame();

	EditorSceneHandle* GetSelectedScene();

private:
	SceneRenderer m_SceneRenderer;
	InputController m_InputController;

	std::shared_ptr<EditorUniversalHandle> m_SelectedSceneHandle = nullptr;

	std::unique_ptr<GameBase> m_RunningGame = nullptr;

	static constexpr Rgb8 s_NoGameRunningBackground = Rgb8(200, 200, 200);
};

