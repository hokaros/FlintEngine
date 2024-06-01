#include "LabyrinthScene.h"

#include <Firearm.h>
#include <Health.h>

#include <Serialization/SceneLoader.h>

LabyrinthScene::LabyrinthScene(const Vector& player_pos, AssetManager& asset_manager)
	: m_PlayerStartingPos(player_pos)
	, m_AssetManager(asset_manager)
{
	LoadFromFile("Assets/main.scene");
	LoadStartingObjects();
}

void LabyrinthScene::Update()
{
	Scene::Update();
}

void LabyrinthScene::Render(SceneRenderer& renderer)
{
	Scene::Render(renderer);

	VectorInt player_label_pos = m_Player->GetPosition() + Vector(-20, -20);
	renderer.RenderString(PLAYER_GO_NAME, player_label_pos, /*fontSize*/10, /*layer*/1);
}

void LabyrinthScene::LoadFromFile(const char* file_path)
{
	std::unique_ptr<Scene> scene = SceneLoader::LoadScene(file_path)->CreateRuntimeObject();
	MoveObjectsFrom(std::move(*scene));
	
	m_BackgroundColor = scene->GetBackgroundColor();
}

void LabyrinthScene::LoadStartingObjects()
{
	m_HealthStats = FindHealthStats();
	GameObject* lab_object = FindLabyrinth();
	FE_ASSERT(lab_object != nullptr, "No labyrinth object loaded");
	m_Lab = lab_object->FindComponent<LabyrinthSolidifier>();
	m_Player = CreatePlayer(m_PlayerStartingPos);
}

GameObject* LabyrinthScene::CreatePlayer(const Vector& position)
{
	GameObject* player = FindPlayer();
	FE_ASSERT(player != nullptr, "No Player loaded");

	player->SetPosition(position);

	return player;
}

bool LabyrinthScene::ShouldRender(GameObject* go)
{
	// Wy�wietlanie tylko, je�li obiekt jest widziany przez obecnego gracza
	if ((go->GetPosition() - m_Player->GetPosition()).LengthSquared() > PLAYER_SIGHT * PLAYER_SIGHT)
		return false;  // zbyt daleko

	// Sprawdzenie, czy �ciana stoi na drodze
	return !m_Lab->GetColliderMemory().Raycast(
		m_Player->GetMiddle(),
		go->GetMiddle(),
		go
	);
}

GameObject* LabyrinthScene::FindLabyrinth()
{
	return FindGameObjectByName("Labyrinth");
}

GameObject* LabyrinthScene::FindPlayer()
{
	return FindGameObjectByName(PLAYER_GO_NAME);
}

BMPStats* LabyrinthScene::FindHealthStats()
{
	GameObject* go = FindGameObjectByName("Health Stats");
	FE_ASSERT(go != nullptr, "Health Stats not found");

	return go->FindComponent<BMPStats>();
}
