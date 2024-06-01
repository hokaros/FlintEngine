#include "LabyrinthScene.h"

#include <Firearm.h>
#include <Health.h>

#include <Serialization/SceneLoader.h>

LabyrinthScene::LabyrinthScene()
{
	LoadFromFile("Assets/main.scene");
}

void LabyrinthScene::LoadFromFile(const char* file_path)
{
	std::unique_ptr<Scene> scene = SceneLoader::LoadScene(file_path)->CreateRuntimeObject();
	MoveObjectsFrom(std::move(*scene));
	
	m_BackgroundColor = scene->GetBackgroundColor();
}