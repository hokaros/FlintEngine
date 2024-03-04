#include "LabyrinthScene.h"

#include "../LabyrinthShooterBase/Firearm.h"
#include "../LabyrinthShooterBase/Health.h"

#include "../FlatEngine/SceneLoader.h"

static constexpr const char* s_HeartBitmapPath = "resources/heart.bmp";

LabyrinthScene::LabyrinthScene(const Vector& player_pos, AssetManager& asset_manager)
	: m_PlayerStartingPos(player_pos)
	, m_AssetManager(asset_manager)
{
	LoadFromFile("resources/main.scene");
	LoadStartingObjects();
}

void LabyrinthScene::Update()
{
	Scene::Update();

	m_Lab->Update();
}

void LabyrinthScene::Render()
{
	Scene::Render();

	// Renderowanie nak³adek UI
	m_HealthStats->Render();

	VectorInt player_label_pos = m_Player->GetPosition() + Vector(-20, -20);
	SceneRenderer::Main()->DrawStringScreenSpace(player_label_pos.x, player_label_pos.y, "Player", 10);
}

void LabyrinthScene::LoadFromFile(const char* file_path)
{
	std::unique_ptr<Scene> scene = SceneLoader::LoadScene(file_path);
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

	Vector mapStart(10, 10);
	m_Lab = std::make_unique<LabyrinthSolidifier>(mapStart, WALL_THICKNESS, WALL_LENGTH, LAB_X, LAB_Y, LAB_TIME, true);
	for (int i = 0; i < m_Lab->WallsCount(); i++)
	{
		m_ObjectManager.AddToMessageSubscribers(m_Lab->GetWalls()[i]);
	}
	for (int i = 0; i < m_Lab->BorderElements(); i++)
	{
		m_ObjectManager.AddToMessageSubscribers(m_Lab->GetBorder()[i]);
	}

	m_Player = CreatePlayer(m_PlayerStartingPos);
}

GameObject* LabyrinthScene::CreatePlayer(const Vector& position)
{
	GameObject* player = FindPlayer();
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
