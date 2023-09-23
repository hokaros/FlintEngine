#include "Game.h"

Game::Game(Window* window, GameStartInfo&& gameInfo, bool serverVersion)
	: window(window), isServer(serverVersion),
	basicBullet(Vector(4, 4), objectManager.GetAllObjects()), // uwa¿aæ przy zmienianiu objectManagera
	superBullet(Vector(10, 10), objectManager.GetAllObjects()),
	startInfo(std::move(gameInfo)) {

	if (!serverVersion) {
		bitmaps = new GameBitmaps();
		healthStats = new  BMPStats(bitmaps->heartBmp, VectorInt(30, 30), VectorInt(3, 3));
	}
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

Game::~Game() {
	std::lock_guard<std::mutex> lock(playersMutex);
	if (players != NULL) {
		delete[] players; // usuniêcie tablicy wskaŸników
	}
}

void Game::LoadStartingObjects() {
	std::lock_guard<std::mutex> lock(playersMutex);

	// Za³adowanie graczy
	playerCount = startInfo.GetPlayerCount();
	players = new GameObject * [playerCount];

	for (size_t i = 0; i < playerCount; i++) {
		bool isControllable = startInfo.GetControllableIndex() == i && !isServer;
		GameObject* newPlayer = CreatePlayer(startInfo.GetPlayerPosition(i), isControllable);

		players[i] = newPlayer;
		if (isControllable) {
			controlledPlayer = newPlayer;
		}
	}
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
	lab = new LabyrinthSolidifier(mapStart, WALL_THICKNESS, WALL_LENGTH, LAB_X, LAB_Y, objectManager.GetAllObjects(), LAB_TIME, isServer);
	for (int i = 0; i < lab->WallsCount(); i++) {
		objectManager.AddUndestroyable(lab->GetWalls()[i]);
	}
	for (int i = 0; i < lab->BorderElements(); i++) {
		objectManager.AddUndestroyable(lab->GetBorder()[i]);
	}
	lab->onChanged = [this](bool* newWalls) {onLabChanged(newWalls); };

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

GameObject* Game::CreatePlayer(const Vector& position, bool isControlled) {

	// Obiekt gracza
	GameObject* player = new GameObject(Vector(20, 20), position, objectManager.GetAllObjects());
	ConstantMover* mover = new ConstantMover(*player, PLAYER_SPEED);
	player->AddComponent(mover);
	objectManager.AddObject(player);

	// Broñ
	GameObject* basicWeapon = new GameObject(
		Vector(30, 10),
		player->GetPosition() + Vector(Direction::EAST) * player->GetSize().x,
		objectManager.GetAllObjects()
	);
	Firearm* basicFirearm = new Firearm(*basicWeapon, basicBullet, WPN_BASIC_RELOAD, FirearmType::Basic);
	basicFirearm->onPlayerCollision = [this](GameObject& p, int dmg) {OnBulletPlayerHit(p, dmg); };
	basicWeapon->AddComponent(basicFirearm);
	player->AddChild(basicWeapon);
	objectManager.AddObject(basicWeapon);

	// Silna broñ
	GameObject * superWeapon = new GameObject(
		Vector(30, 10),
		player->GetPosition() + Vector(Direction::EAST) * player->GetSize().x,
		objectManager.GetAllObjects()
	);
	Firearm* superFirearm = new Firearm(*superWeapon, superBullet, WPN_SUPER_RELOAD, FirearmType::Super);
	superFirearm->onPlayerCollision = [this](GameObject& p, int dmg) {OnBulletPlayerHit(p, dmg); };
	superWeapon->AddComponent(superFirearm);
	player->AddChild(superWeapon);
	objectManager.AddObject(superWeapon);

	// Ekwipunek
	player->AddComponent(new PlayerEquipment(*player));
	// Zdrowie
	StatRenderer* healthRenderer = NULL;
	if (isControlled && healthStats != NULL) { // Wyœwietlamy zdrowie tylko gracza kontrolowanego przez tego klienta
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

	if (isControlled) {
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
	}

	if (window != NULL) {
		SDL_Surface* screen = window->GetScreen();

		player->SetRenderer(new SpriteRenderer(*player, screen, bitmaps->playerBmp));
		basicWeapon->SetRenderer(new SpriteRenderer(*basicWeapon, screen, bitmaps->wpnBasicBmp));
		superWeapon->SetRenderer(new SpriteRenderer(*superWeapon, screen, bitmaps->wpnSuperBmp));
	}

	return player;
}

GameObject* Game::GetControlledPlayer() {
	std::lock_guard<std::mutex> lock(playersMutex);
	return controlledPlayer;
}

GameObject* Game::GetPlayer(int id) {
	std::lock_guard<std::mutex> lock(playersMutex);

	if (players == NULL || playerCount <= id)
		return NULL;

	return players[id];
}

int Game::GetPlayerIndex(GameObject* player) const {
	for (int i = 0; i < playerCount; i++) {
		if (players[i] == player)
			return i;
	}

	return -1;
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
	if (!isServer)
		return;

	printf("A player got hit for %d hp\n", dmg);
	int id = GetPlayerIndex(&player);
	if (id == -1) {
		printf("I don't know this guy\n");
		return;
	}

	player.FindComponent<Health>()->Hurt(dmg);

	if(onPlayerHurt)
		onPlayerHurt(id, dmg);
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
		if (go->renderUnseen || controlledPlayer == NULL || controlledPlayer->FindComponent<Health>()->IsDead()) {
			go->RenderUpdate();
			continue;
		}

		// Wyœwietlanie tylko, jeœli obiekt jest widziany przez obecnego gracza
		if ((go->GetPosition() - controlledPlayer->GetPosition()).LengthSquared() > PLAYER_SIGHT * PLAYER_SIGHT)
			continue;  // zbyt daleko

		// Sprawdzenie, czy œciana stoi na drodze
		bool canSee = !lab->GetColliderMemory().Raycast(
			controlledPlayer->GetMiddle(),
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



GameStartInfo::GameStartInfo(Vector* playerPositions, size_t playerCount, int controllableIndex)
	: playerPositions(playerPositions), playerCount(playerCount), controllableIndex(controllableIndex) {

}

GameStartInfo::GameStartInfo(GameStartInfo&& other)
	: playerPositions(other.playerPositions), playerCount(other.playerCount), controllableIndex(other.controllableIndex) {
	other.playerPositions = NULL;
	other.playerCount = 0;
}

GameStartInfo::~GameStartInfo() {
	if (playerPositions != NULL) {
		delete[] playerPositions;
	}
}

Vector GameStartInfo::GetPlayerPosition(int index) const {
	return playerPositions[index];
}

size_t GameStartInfo::GetPlayerCount() const {
	return playerCount;
}

int GameStartInfo::GetControllableIndex() const {
	return controllableIndex;
}