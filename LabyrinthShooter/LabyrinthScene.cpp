#include "LabyrinthScene.h"

#include "../LabyrinthShooterBase/Firearm.h"
#include "../LabyrinthShooterBase/Health.h"

static constexpr const char* s_PlayerBitmapPath = "resources/player.bmp";
static constexpr const char* s_HeartBitmapPath = "resources/heart.bmp";

LabyrinthScene::LabyrinthScene(const Vector& player_pos, PrefabFactory& prefab_factory, AssetManager& asset_manager)
	: m_PlayerStartingPos(player_pos)
	, m_PrefabFactory(prefab_factory)
	, m_AssetManager(asset_manager)
{
	LoadNeededAssets();
	LoadStartingObjects();

	m_BackgroundColor = Rgb8(0, 0, 0);
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
	Window::Main()->DrawString(player_label_pos.x, player_label_pos.y, "Player", 10);
}

void LabyrinthScene::LoadNeededAssets()
{
	m_AssetManager.AddAsset(s_PlayerBitmapPath);
	m_AssetManager.AddAsset(s_HeartBitmapPath);
}

void LabyrinthScene::LoadStartingObjects()
{
	SDL_Surface* heart_bitmap = m_AssetManager.GetSurfaceAsset(s_HeartBitmapPath);
	m_HealthStats = std::make_unique<BMPStats>(heart_bitmap, VectorInt(30, 30), VectorInt(3, 3));

	Vector mapStart(10, 10);
	m_Lab = std::make_unique<LabyrinthSolidifier>(mapStart, WALL_THICKNESS, WALL_LENGTH, LAB_X, LAB_Y, LAB_TIME, true);
	for (int i = 0; i < m_Lab->WallsCount(); i++)
	{
		m_ObjectManager.AddUndestroyable(m_Lab->GetWalls()[i]);
	}
	for (int i = 0; i < m_Lab->BorderElements(); i++)
	{
		m_ObjectManager.AddUndestroyable(m_Lab->GetBorder()[i]);
	}

	m_Player = CreatePlayer(m_PlayerStartingPos);
}

GameObject* LabyrinthScene::CreatePlayer(const Vector& position)
{
	GameObject* player = GameObject::Instantiate(m_PrefabFactory.GetPrefab(PrefabFactory::EPrefabId::Player));
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
