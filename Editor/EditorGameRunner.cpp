#include "EditorGameRunner.h"

void EditorGameRunner::SetSelectedScene(std::shared_ptr<EditorUniversalHandle> handle)
{
	StopGame();

	m_SelectedSceneHandle = handle;
}

void EditorGameRunner::Render()
{
	if (m_IsGameRunning)
	{
		FE_LOG("TODO: Game loop");
	}

	if (ImGui::Begin("Game"))
	{
		RenderControlPanel();

		// TODO: render rendering output
	}
	ImGui::End();
}

void EditorGameRunner::StartGame()
{
	if (m_IsGameRunning)
		return;
	
	// TODO:: init game
	FE_LOG("TODO: init game");

	m_IsGameRunning = true;
}

void EditorGameRunner::StopGame()
{
	if (!m_IsGameRunning)
		return;

	// TODO: deinit game
	FE_LOG("TODO: deinit game");

	m_IsGameRunning = false;
}

void EditorGameRunner::RenderControlPanel()
{
	const bool has_runnable_scene = GetSelectedScene() != nullptr;

	ImGui::BeginDisabled(!has_runnable_scene);
	{
		ImGui::BeginDisabled(m_IsGameRunning); // Cannot run if the game is running
		if (ImGui::Button("Run"))
		{
			StartGame();
		}
		ImGui::EndDisabled();

		ImGui::SameLine();

		ImGui::BeginDisabled(!m_IsGameRunning);
		if (ImGui::Button("Stop"))
		{
			StopGame();
		}
		ImGui::EndDisabled();
	}
	ImGui::EndDisabled();
}

EditorSceneHandle* EditorGameRunner::GetSelectedScene()
{
	if (m_SelectedSceneHandle == nullptr)
		return nullptr;

	return m_SelectedSceneHandle->GetSceneHandle();
}
