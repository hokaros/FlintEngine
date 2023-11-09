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

	GameObject* player = GameObject::Instantiate(m_PrefabFactory.GetPrefab(PrefabFactory::EPrefabId::Player));
	player->SetPosition(position);

	std::list<Firearm*>* firearms = player->FindComponentsInChildren<Firearm>();
	for (Firearm* firearm : *firearms)
	{
		firearm->onPlayerCollision = [this](GameObject& p, int dmg) {OnBulletPlayerHit(p, dmg); };
	}
	delete firearms;

	Health* player_health = player->FindComponent<Health>();
	player_health->SetStatRenderer(healthStats);
	player_health->SubscribeDeath(
		[](Health* deadPlayer) {
			printf("Dead\n");
			GameObject::Destroy(&(deadPlayer->GetOwner()));
		}
	);

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