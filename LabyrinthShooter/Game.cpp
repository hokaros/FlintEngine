#include "Game.h"

Game::Game(Window* window, GameStartInfo&& gameInfo)
	: window(window)
	, basicBullet(Vector(4, 4), objectManager.GetAllObjects(), {}) // uwa¿aæ przy zmienianiu objectManagera
	, superBullet(Vector(10, 10), objectManager.GetAllObjects(), {})
	, startInfo(std::move(gameInfo)) {


	bitmaps = new GameBitmaps();
	healthStats = new  BMPStats(bitmaps->heartBmp, VectorInt(30, 30), VectorInt(3, 3));

	if (window != NULL) {
		SDL_Surface* screen = window->GetScreen();

		int red = SDL_MapRGB(screen->format, 0xFF, 0x00, 0x00);
		int yellow = SDL_MapRGB(screen->format, 0xFF, 0xFF, 0x00);

		basicBullet.SetRenderer(new RectangleRenderer(basicBullet, screen, yellow, yellow));
		superBullet.SetRenderer(new RectangleRenderer(superBullet, screen, red, red));
	}

	basicBullet.AddComponent(new Bullet(basicBullet, BULLET_BASIC_SPEED, BULLET_BASIC_DAMAGE));
	superBullet.AddComponent(new PowerBullet(superBullet, BULLET_SUPER_SPEED, BULLET_SUPER_DAMAGE));
}

void Game::LoadStartingObjects() {
	std::lock_guard<std::mutex> lock(playersMutex);

	m_Player = CreatePlayer(startInfo.GetPlayerPosition());
}

bool Game::Run() {
	InputController* input = InputController::Main();

	int black = 0;
	SDL_Surface* screen = NULL;
	if (window != NULL) {
		screen = window->GetScreen();
		black = SDL_MapRGB(screen->format, 0x00, 0x00, 0x00);
	}

	Vector mapStart(10, 10);
	lab = new LabyrinthSolidifier(mapStart, WALL_THICKNESS, WALL_LENGTH, LAB_X, LAB_Y, objectManager.GetAllObjects(), LAB_TIME, true);
	for (int i = 0; i < lab->WallsCount(); i++) {
		objectManager.AddUndestroyable(lab->GetWalls()[i]);
	}
	for (int i = 0; i < lab->BorderElements(); i++) {
		objectManager.AddUndestroyable(lab->GetBorder()[i]);
	}
	//lab->onChanged = [this](bool* newWalls) {onLabChanged(newWalls); };

	LoadStartingObjects();

	int quit = 0;

	timer.NextFrame();

	for (GameObject* go : objectManager.GetAllObjects()) {
		go->Start();
	}

	SetRunning(true);
	// Pêtla gry
	while (!quit) {
		// Nowa klatka
		timer.NextFrame();

		if (input != NULL && !input->Update())
			return false;

		if (input != NULL && input->PressedThisFrame(SDLK_ESCAPE))
			quit = 1;

		//generowanie t³a
		if(screen != NULL)
			SDL_FillRect(screen, NULL, black);

		// Wywo³anie zleconych akcji
		InvokePostponed();

		// Zaktualizowanie stanu gry
		for (GameObject* go : objectManager.GetAllObjects()) {
			go->Update();
		}
		lab->Update();

		// Renderowanie obiektów
		if (window != NULL) {
			Render();
			window->Render();
		}

		objectManager.DisposeDestroyed();
	}

	SetRunning(false);
	delete lab;
	return true;
}

void Game::Clear() {
	objectManager.Clear();
}

GameObject* Game::CreatePlayer(const Vector& position) {

	// Obiekt gracza
	GameObject* player = GameObject::Instantiate(Vector(20, 20), position);
	ConstantMover* mover = new ConstantMover(*player, PLAYER_SPEED);
	player->AddComponent(mover);

	// Broñ
	GameObject* basicWeapon = GameObject::Instantiate(
		Vector(30, 10),
		player->GetPosition() + Vector(Direction::EAST) * player->GetSize().x
	);
	Firearm* basicFirearm = new Firearm(*basicWeapon, basicBullet, WPN_BASIC_RELOAD, FirearmType::Basic);
	basicFirearm->onPlayerCollision = [this](GameObject& p, int dmg) {OnBulletPlayerHit(p, dmg); };
	basicWeapon->AddComponent(basicFirearm);
	player->AddChild(basicWeapon);

	// Silna broñ
	GameObject * superWeapon = GameObject::Instantiate(
		Vector(30, 10),
		player->GetPosition() + Vector(Direction::EAST) * player->GetSize().x
	);
	Firearm* superFirearm = new Firearm(*superWeapon, superBullet, WPN_SUPER_RELOAD, FirearmType::Super);
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
		[&](Health* deadPlayer) {
			printf("Dead\n");
			objectManager.DestroyObject(&(deadPlayer->GetOwner()));
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
		SDL_Surface* screen = window->GetScreen();

		player->SetRenderer(new SpriteRenderer(*player, screen, bitmaps->playerBmp));
		basicWeapon->SetRenderer(new SpriteRenderer(*basicWeapon, screen, bitmaps->wpnBasicBmp));
		superWeapon->SetRenderer(new SpriteRenderer(*superWeapon, screen, bitmaps->wpnSuperBmp));
	}

	return player;
}

GameObject* Game::GetPlayer() {
	return m_Player;
}

LabyrinthSolidifier* Game::GetLab() const {
	return lab;
}

void Game::InvokeOnNextFrame(function<void()> fun) {
	std::lock_guard<std::mutex> lock(invokesMutex);
	invokes.push_back(std::move(fun));
}

void Game::InvokePostponed() {
	std::lock_guard<std::mutex> lock(invokesMutex);
	for (function<void()> fun : invokes) {
		if (fun)
			fun();
	}
	invokes.clear();
}

void Game::OnControlledDirectionChanged(const Vector& newDir) {
	if (onControlledDirectionChanged)
		onControlledDirectionChanged(newDir);
}

void Game::OnBulletPlayerHit(GameObject& player, int dmg) {
	printf("A player got hit for %d hp\n", dmg);

	player.FindComponent<Health>()->Hurt(dmg);
}

bool Game::IsRunning() {
	std::lock_guard<std::mutex> lock(metadataMutex);
	return isRunning;
}

void Game::SetRunning(bool running) {
	std::lock_guard<std::mutex> lock(metadataMutex);
	isRunning = running;
}

void Game::Render() {
	for (GameObject* go : objectManager.GetAllObjects()) {
		if (go->renderUnseen) {
			go->RenderUpdate();
			continue;
		}

		// Wyœwietlanie tylko, jeœli obiekt jest widziany przez obecnego gracza
		if ((go->GetPosition() - m_Player->GetPosition()).LengthSquared() > PLAYER_SIGHT * PLAYER_SIGHT)
			continue;  // zbyt daleko

		// Sprawdzenie, czy œciana stoi na drodze
		bool canSee = !lab->GetColliderMemory().Raycast(
			m_Player->GetMiddle(),
			go->GetMiddle(),
			go
		);
		if (canSee) {
			go->RenderUpdate();
		}
	}
	// Renderowanie nak³adek UI
	healthStats->Render();
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