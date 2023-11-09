#include "Game.h"

static constexpr const char* s_PlayerBitmapPath = "resources/player.bmp";
static constexpr const char* s_HeartBitmapPath = "resources/heart.bmp";

Game::Game(Window* window, GameStartInfo&& gameInfo)
	: GameBase(window)
	, startInfo(std::move(gameInfo)) 
{
	m_AssetManager.AddAsset(s_PlayerBitmapPath);
	m_AssetManager.AddAsset(s_HeartBitmapPath);
	SDL_Surface* heart_bitmap = m_AssetManager.GetSurfaceAsset(s_HeartBitmapPath);
	healthStats = new  BMPStats(heart_bitmap, VectorInt(30, 30), VectorInt(3, 3));
}

void Game::LoadStartingObjects() {
	std::lock_guard<std::mutex> lock(playersMutex);

	m_Player = CreatePlayer(startInfo.GetPlayerPosition());
}

GameObject* Game::CreatePlayer(const Vector& position) {

	// Obiekt gracza
	Vector player_size = Vector(20, 20);
	GameObject* player = GameObject::Instantiate(player_size, position);
	ConstantMover* mover = new ConstantMover(*player, PLAYER_SPEED);
	player->AddComponent(mover);
	player->AddComponent(new BoxCollider(*player, Vector::ZERO, player_size));

	// Broñ
	const GameObject& basic_bullet = m_PrefabFactory.GetPrefab(PrefabFactory::EPrefabId::BasicBullet);
	const GameObject& super_bullet = m_PrefabFactory.GetPrefab(PrefabFactory::EPrefabId::SuperBullet);

	// Zwyk³a broñ
	GameObject* basic_weapon = GameObject::Instantiate(
		m_PrefabFactory.GetPrefab(PrefabFactory::EPrefabId::BasicFirearm)
	);
	basic_weapon->SetPosition(player->GetPosition() + Vector(Direction::EAST) * player->GetSize().x);
	basic_weapon->FindComponent<Firearm>()->onPlayerCollision = [this](GameObject& p, int dmg) {OnBulletPlayerHit(p, dmg); };
	player->AddChild(basic_weapon);

	// Silna broñ
	GameObject* super_weapon = GameObject::Instantiate(
		m_PrefabFactory.GetPrefab(PrefabFactory::EPrefabId::SuperFirearm)
	);
	super_weapon->SetPosition(player->GetPosition() + Vector(Direction::EAST) * player->GetSize().x);
	super_weapon->FindComponent<Firearm>()->onPlayerCollision = [this](GameObject& p, int dmg) {OnBulletPlayerHit(p, dmg); };
	player->AddChild(super_weapon);

	// Ekwipunek
	player->AddComponent(new PlayerEquipment(*player));
	// Zdrowie
	StatRenderer* healthRenderer = NULL;
	if (healthStats != NULL) {
		healthRenderer = healthStats;
	}
	Health* playerHealth = new Health(*player, MAX_HEALTH, healthRenderer);
	player->AddComponent(playerHealth);
	playerHealth->SubscribeDeath(
		[](Health* deadPlayer) {
			printf("Dead\n");
			GameObject::Destroy(&(deadPlayer->GetOwner()));
		}
	);

	PlayerController* controller = new PlayerController(*player);
	player->AddComponent(controller);

	if (window != NULL) {
		SDL_Surface* player_bitmap = m_AssetManager.GetSurfaceAsset(s_PlayerBitmapPath);
		player->SetRenderer(new SpriteRenderer(*player, player_bitmap));
	}

	return player;
}

GameObject* Game::GetPlayer() {
	return m_Player;
}

LabyrinthSolidifier* Game::GetLab() const {
	return lab;
}

void Game::OnBulletPlayerHit(GameObject& player, int dmg) {
	printf("A player got hit for %d hp\n", dmg);

	player.FindComponent<Health>()->Hurt(dmg);
}

void Game::PreRun()
{
	Vector mapStart(10, 10);
	lab = new LabyrinthSolidifier(mapStart, WALL_THICKNESS, WALL_LENGTH, LAB_X, LAB_Y, LAB_TIME, true);
	for (int i = 0; i < lab->WallsCount(); i++) {
		objectManager.AddUndestroyable(lab->GetWalls()[i]);
	}
	for (int i = 0; i < lab->BorderElements(); i++) {
		objectManager.AddUndestroyable(lab->GetBorder()[i]);
	}

	LoadStartingObjects();
}

void Game::PostRun()
{
	delete lab;
}

void Game::PrePresent()
{
	// Renderowanie nak³adek UI
	healthStats->Render();

	VectorInt player_label_pos = GetPlayer()->GetPosition() + Vector(-20, -20);
	Window::Main()->DrawString(player_label_pos.x, player_label_pos.y, "Player", 10);
}

void Game::PostObjectsUpdate()
{
	lab->Update();
}

bool Game::ShouldRender(GameObject* go)
{
	// Wyœwietlanie tylko, jeœli obiekt jest widziany przez obecnego gracza
	if ((go->GetPosition() - m_Player->GetPosition()).LengthSquared() > PLAYER_SIGHT * PLAYER_SIGHT)
		return false;  // zbyt daleko

	// Sprawdzenie, czy œciana stoi na drodze
	return !lab->GetColliderMemory().Raycast(
		m_Player->GetMiddle(),
		go->GetMiddle(),
		go
	);
}




GameStartInfo::GameStartInfo(const Vector& player_position)
	: m_PlayerPosition(player_position) 
{

}

const Vector& GameStartInfo::GetPlayerPosition() const {
	return m_PlayerPosition;
}