#pragma once
#include <imgui/imgui.h>

#include <InputController.h>
#include <Core/Scene.h>
#include <Core/GameBase.h>
#include <AssetHandles/EditorUniversalHandle.h>

class EditorInputController
	: public InputController
{
public:
	virtual Vector GetMousePosition() const override;

	void UpdateMousePosition(const Rect& viewport);

private:
	Vector m_MousePos;
};

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
	EditorInputController m_InputController;

	std::shared_ptr<EditorUniversalHandle> m_SelectedSceneHandle = nullptr;

	std::unique_ptr<GameBase> m_RunningGame = nullptr;

	static constexpr Rgb8 s_NoGameRunningBackground = Rgb8(200, 200, 200);
};

