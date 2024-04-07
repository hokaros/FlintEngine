#include "EditorGameRunner.h"

EditorGameRunner::EditorGameRunner(SDL_Renderer& renderer, float screenWidth, float screenHeight)
	: m_SceneRenderer(screenWidth, screenHeight)
{
	m_SceneRenderer.Init(&renderer, {});
}

void EditorGameRunner::SetSelectedScene(std::shared_ptr<EditorUniversalHandle> handle)
{
	StopGame();

	m_SelectedSceneHandle = handle;
}

void EditorGameRunner::Render()
{
	if (m_RunningGame != nullptr)
	{
		m_RunningGame->RunOneLoop();
	}
	else
	{
		ClearGameFrame();
	}

	if (ImGui::Begin("Game"))
	{
		RenderControlPanel();
		RenderGameFrame();
	}
	ImGui::End();
}

void EditorGameRunner::StartGame()
{
	if (m_RunningGame != nullptr)
		return;
	
	EditorSceneHandle* scene_to_run = GetSelectedScene();
	if (scene_to_run == nullptr)
	{
		FE_WARN("No scene to run");
		return;
	}

	m_RunningGame = std::make_unique<GameBase>(/*window*/nullptr, &m_SceneRenderer, m_InputController);
	m_RunningGame->LoadScene(
		scene_to_run->GetScene().CreateRuntimeObject()
	);
}

void EditorGameRunner::StopGame()
{
	m_RunningGame = nullptr;
}

bool EditorGameRunner::IsGameRunning() const
{
	return m_RunningGame != nullptr;
}

InputController& EditorGameRunner::GetInputController()
{
	return m_InputController;
}

void EditorGameRunner::RenderControlPanel()
{
	const bool has_runnable_scene = GetSelectedScene() != nullptr;
	const bool is_game_running = m_RunningGame != nullptr;

	ImGui::BeginDisabled(!has_runnable_scene);
	{
		ImGui::BeginDisabled(is_game_running); // Cannot run if the game is running
		if (ImGui::Button("Run"))
		{
			StartGame();
		}
		ImGui::EndDisabled();

		ImGui::SameLine();

		ImGui::BeginDisabled(!is_game_running);
		if (ImGui::Button("Stop"))
		{
			StopGame();
		}
		ImGui::EndDisabled();
	}
	ImGui::EndDisabled();
}

void EditorGameRunner::RenderGameFrame()
{
	if (SDL_Texture* renderedTex = m_SceneRenderer.GetOutputTexture())
	{
		ImVec2 renderSize = ImGui::GetContentRegionAvail();
		ImGui::Image((void*)renderedTex, renderSize);
	}
	else
	{
		FE_ASSERT(false, "No scene rendered");
	}
}

void EditorGameRunner::ClearGameFrame()
{
	m_SceneRenderer.Clear(s_NoGameRunningBackground);
}

EditorSceneHandle* EditorGameRunner::GetSelectedScene()
{
	if (m_SelectedSceneHandle == nullptr)
		return nullptr;

	return m_SelectedSceneHandle->GetSceneHandle();
}
