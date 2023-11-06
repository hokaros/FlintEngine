#include "Game.h"

Vector Game::s_BasicBulletSize = Vector(4, 4);
Vector Game::s_SuperBulletSize = Vector(10, 10);

Game::Game(Window* window, GameStartInfo&& gameInfo)
	: GameBase(window)
	, startInfo(std::move(gameInfo)) 
{
	bitmaps = new GameBitmaps();
	healthStats = new  BMPStats(bitmaps->heartBmp, VectorInt(30, 30), VectorInt(3, 3));
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

	GameObject* basicWeapon = GameObject::Instantiate(
		Vector(30, 10),
		player->GetPosition() + Vector(Direction::EAST) * player->GetSize().x
	);
	Firearm* basicFirearm = new Firearm(*basicWeapon, basic_bullet, WPN_BASIC_RELOAD, FirearmType::Basic);
	basicFirearm->onPlayerCollision = [this](GameObject& p, int dmg) {OnBulletPlayerHit(p, dmg); };
	basicWeapon->AddComponent(basicFirearm);
	player->AddChild(basicWeapon);

	// Silna broñ
	GameObject * superWeapon = GameObject::Instantiate(
		Vector(30, 10),
		player->GetPosition() + Vector(Direction::EAST) * player->GetSize().x
	);
	Firearm* superFirearm = new Firearm(*superWeapon, super_bullet, WPN_SUPER_RELOAD, FirearmType::Super);
	superFirearm->onPlayerCollision = [this](GameObject& p, int dmg) {OnBulletPlayerHit(p, dmg); };
	superWeapon->AddComponent(superFirearm);
	player->AddChild(superWeapon);

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
	controller->onAimChanged = [this](double newRot) {
		if (onControlledAimChanged) 
			onControlledAimChanged(newRot); 
	};
	controller->onShot = [this]() {
		if (onControlledShot)
			onControlledShot();
	};
	controller->onWeaponChanged = [this](FirearmType newType) {
		if (onControlledWeaponChanged)
			onControlledWeaponChanged(newType);
	};
	mover->onDirectionChanged = [this](const Vector& newDir) {OnControlledDirectionChanged(newDir); };

	if (window != NULL) {
		player->SetRenderer(new SpriteRenderer(*player, bitmaps->playerBmp));
		basicWeapon->SetRenderer(new SpriteRenderer(*basicWeapon, bitmaps->wpnBasicBmp));
		superWeapon->SetRenderer(new SpriteRenderer(*superWeapon, bitmaps->wpnSuperBmp));
	}

	return player;
}

GameObject* Game::GetPlayer() {
	return m_Player;
}

LabyrinthSolidifier* Game::GetLab() const {
	return lab;
}

void Game::OnControlledDirectionChanged(const Vector& newDir) {
	if (onControlledDirectionChanged)
		onControlledDirectionChanged(newDir);
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




GameBitmaps::GameBitmaps() {
	playerBmp = SDL_LoadBMP("resources/player.bmp");
	if (playerBmp == NULL) {
		printf("Nie udalo sie zaladowac resources/player.bmp\n");
		bitmapsOk = false;
	}

	wpnBasicBmp = SDL_LoadBMP("resources/weapon_primary.bmp");
	if (wpnBasicBmp == NULL) {
		printf("Nie udalo sie zaladowac resources/weapon_primary.bmp\n");
		bitmapsOk = false;
	}

	wpnSuperBmp = SDL_LoadBMP("resources/weapon_super.bmp");
	if (wpnSuperBmp == NULL) {
		printf("Nie udalo sie zaladowac resources/weapon_super.bmp\n");
		bitmapsOk = false;
	}

	heartBmp = SDL_LoadBMP("resources/heart.bmp");
	if (heartBmp == NULL) {
		printf("Nie udalo sie zaladowac resources/heart.bmp\n");
		bitmapsOk = false;
	}
}

GameBitmaps::~GameBitmaps() {
	SDL_FreeSurface(playerBmp);
	SDL_FreeSurface(wpnBasicBmp);
	SDL_FreeSurface(wpnSuperBmp);
}

bool GameBitmaps::IsOk() const {
	return bitmapsOk;
}



GameStartInfo::GameStartInfo(const Vector& player_position)
	: m_PlayerPosition(player_position) 
{

}

const Vector& GameStartInfo::GetPlayerPosition() const {
	return m_PlayerPosition;
}