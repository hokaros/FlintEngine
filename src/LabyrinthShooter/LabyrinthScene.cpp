#include "LabyrinthScene.h"

#include <Firearm.h>
#include <Health.h>

#include <Serialization/SceneLoader.h>

LabyrinthScene::LabyrinthScene()
{
	LoadFromFile("Assets/main.scene");
	LoadStartingObjects();
}

void LabyrinthScene::LoadFromFile(const char* file_path)
{
	std::unique_ptr<Scene> scene = SceneLoader::LoadScene(file_path)->CreateRuntimeObject();
	MoveObjectsFrom(std::move(*scene));
	
	m_BackgroundColor = scene->GetBackgroundColor();
}

void LabyrinthScene::LoadStartingObjects()
{
	GameObject* lab_object = FindLabyrinth();
	FE_ASSERT(lab_object != nullptr, "No labyrinth object loaded");
	m_Lab = lab_object->FindComponent<LabyrinthSolidifier>();
	m_Player = FindPlayer();
}

GameObject* LabyrinthScene::FindLabyrinth()
{
	return FindGameObjectByName("Labyrinth");
}

GameObject* LabyrinthScene::FindPlayer()
{
	return FindGameObjectByName(PLAYER_GO_NAME);
}
