#include "LabyrinthScene.h"

#include <Firearm.h>
#include <Health.h>

#include <Serialization/SceneLoader.h>

static constexpr const char* s_HeartBitmapPath = "heart.bmp";

LabyrinthScene::LabyrinthScene(const Vector& player_pos, AssetManager& asset_manager)
	: m_PlayerStartingPos(player_pos)
	, m_AssetManager(asset_manager)
{
	LoadFromFile("Assets/main.scene"); // TODO: get from asset manager
	LoadStartingObjects();
}

void LabyrinthScene::Update()
{
	Scene::Update();
}

void LabyrinthScene::Render(SceneRenderer& renderer)
{
	Scene::Render(renderer);

	// Renderowanie nak³adek UI
	m_HealthStats->Render(renderer);

	VectorInt player_label_pos = m_Player->GetPosition() + Vector(-20, -20);
	renderer.DrawStringScreenSpace(player_label_pos.x, player_label_pos.y, "Player", 10);
}

void LabyrinthScene::LoadFromFile(const char* file_path)
{
	std::unique_ptr<Scene> scene = SceneLoader::LoadScene(file_path)->CreateRuntimeObject();
	for (std::unique_ptr<GameObject>& go : scene->GetObjectManager().GetOwnedObjects())
	{
		GetObjectManager().AddNewObject(std::move(go));
	}
	
	m_BackgroundColor = scene->GetBackgroundColor();
}

void LabyrinthScene::LoadStartingObjects()
{
	SDL_Surface* heart_bitmap = m_AssetManager.GetSurfaceAsset(s_HeartBitmapPath);
	m_HealthStats = std::make_unique<BMPStats>(heart_bitmap, VectorInt(30, 30), VectorInt(3, 3));

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

	std::list<Firearm*>* firearms = player->FindComponentsInChildren<Firearm>();
	for (Firearm* firearm : *firearms)
	{
		firearm->onPlayerCollision = [](GameObject& p, int dmg) {p.FindComponent<Health>()->Hurt(dmg); };
	}
	delete firearms;

	Health* player_health = player->FindComponent<Health>();
	player_health->SetStatRenderer(m_HealthStats.get());
	player_health->SubscribeDeath(
		[](Health* deadPlayer) {
			printf("Dead\n");
			GameObject::Destroy(&(deadPlayer->GetOwner()));
		}
	);

	return player;
}

bool LabyrinthScene::ShouldRender(GameObject* go)
{
	// Wyœwietlanie tylko, jeœli obiekt jest widziany przez obecnego gracza
	if ((go->GetPosition() - m_Player->GetPosition()).LengthSquared() > PLAYER_SIGHT * PLAYER_SIGHT)
		return false;  // zbyt daleko

	// Sprawdzenie, czy œciana stoi na drodze
	return !m_Lab->GetColliderMemory().Raycast(
		m_Player->GetMiddle(),
		go->GetMiddle(),
		go
	);
}

GameObject* LabyrinthScene::FindLabyrinth()
{
	for (std::unique_ptr<GameObject>& go : m_ObjectManager.GetOwnedObjects())
	{
		if (go->GetName() == "Labyrinth")
		{
			return go.get();
		}
	}

	return nullptr;
}

GameObject* LabyrinthScene::FindPlayer()
{
	for (std::unique_ptr<GameObject>& go : m_ObjectManager.GetOwnedObjects())
	{
		if (go->GetName() == "Player")
		{
			return go.get();
		}
	}

	return nullptr;
}
