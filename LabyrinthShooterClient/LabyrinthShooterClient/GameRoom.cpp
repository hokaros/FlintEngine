#include "GameRoom.h"
#include <PlayerPositionsGenerator.h>

GameRoom::GameRoom(Window& window, ClientSinglePlayer* client)
	: window(window), playerCount(1), client(client) {
	SubscribeToClient();
}

void GameRoom::Enter() {
	SDL_Surface* screen = window.GetScreen();
	int black = SDL_MapRGB(screen->format, 0x00, 0x00, 0x00);

	InputController* input = InputController::Main();

	bool quit = false;
	bool shouldStart = false;
	StartGameServer();

	while (!quit) {
		if (!input->Update()) {
			quit = 1;
		}

		// Generowanie t³a
		SDL_FillRect(screen, NULL, black);

		// Wyœwietlenie poczekalni
		DrawWaitingRoom();

		{
			// Odczytanie zewnêtrznego sygna³u rozpoczêcia
			std::lock_guard<std::mutex> lock(mutex);
			shouldStart = startGame;
		}

		if (shouldStart) {
			// Wejœcie do gry
			RunGame();
			std::lock_guard<std::mutex> lock(mutex);
			startGame = false;
		}

		//if (input->PressedThisFrame(SDLK_RETURN)) {
		//	// Testowe wejœcie do gry
		//	SetNextGame(new Game(window, 
		//		GameStartInfo(new Vector[1]{ Vector(50, 200) }, 1, 0))
		//	);
		//	RunGame();
		//}
		if (input->PressedThisFrame(SDLK_ESCAPE)) {
			quit = true;
		}

		window.Render();
	}
}

void GameRoom::DrawWaitingRoom() {
	char buffer[16];
	// Informacja o liczbie graczy w poczekalni
	sprintf_s(buffer, "%d/%d", GetPlayerCount(), PLAYERS_NUM);
	window.DrawString(5, 5, buffer, FONTSIZE_MEDIUM);

	window.DrawString(window.GetWidth()/2 - 15*FONTSIZE_MEDIUM, window.GetHeight()/2 - FONTSIZE_MEDIUM, "Oczekiwanie na graczy...", FONTSIZE_MEDIUM);
}

void GameRoom::StartGame(int selfId, Vector* playerPositions, size_t playerCount) {
	this->playerCount = playerCount;

	Vector* positionsCpy = new Vector[playerCount];
	std::memcpy(positionsCpy, playerPositions, playerCount * sizeof(Vector));
	GameStartInfo gameInfo(positionsCpy, playerCount, selfId);

	std::lock_guard<std::mutex> lock(mutex);
	nextGame = new Game(&window, std::move(gameInfo));
	startGame = true;
}

void GameRoom::StartGameServer()
{
	PlayerPositionsGenerator positionsGenerator;
	positionsGenerator.Generate(PLAYERS_NUM, WIDTH, HEIGHT, MAP_START_X, MAP_START_Y);

	std::cout << "Game Started\n";

	std::vector<Vector> positions = positionsGenerator.GetPlayerPositions();
	Vector* positions_arr = new Vector[positions.size()];
	for (size_t i = 0; i < positions.size(); i++)
	{
		positions_arr[i] = positions[i];
	}
	StartGame(0, positions_arr, 1);
}

void GameRoom::RunGame() {
	if (nextGame == NULL)
		return;

	{
		std::lock_guard<std::mutex> lock(mutex);
		// Ustawienie przygotowanej gry jako aktualna
		if (game != NULL) {
			delete game;
			game = NULL;
		}
		game = nextGame;
		nextGame = NULL;
	}

	SubscribeToGame();
	game->Run();

	delete game;
	game = NULL;
}

int GameRoom::GetPlayerCount() {
	std::lock_guard<std::mutex> lock(mutex);

	return playerCount;
}

void GameRoom::SetPlayerCount(int count) {
	std::lock_guard<std::mutex> lock(mutex);

	playerCount = count;
}

void GameRoom::SetShouldStart(bool shouldStart) {
	std::lock_guard<std::mutex> lock(mutex);

	startGame = shouldStart;
}

bool GameRoom::ShouldStart() {
	std::lock_guard<std::mutex> lock(mutex);
	return startGame;
}

void GameRoom::SetNextGame(Game* newGame) {
	std::lock_guard<std::mutex> lock(mutex);
	nextGame = newGame;
}

void GameRoom::OnPlayerJoined() {
	SetPlayerCount(GetPlayerCount() + 1);
}

void GameRoom::OnPlayerLeft() {
	SetPlayerCount(GetPlayerCount() - 1);
}

void GameRoom::OnGameStarted(int selfId, float positions[PLAYERS_NUM][2]) {
	Vector playerPositions[PLAYERS_NUM];
	for (int i = 0; i < PLAYERS_NUM; i++) {
		playerPositions[i] = Vector(positions[i][0], positions[i][1]);
	}
	StartGame(selfId, playerPositions, PLAYERS_NUM);
}

void GameRoom::SubscribeToClient() {
	client->onPlayerJoined = [this]() {OnPlayerJoined(); };
	client->onPlayerLeft = [this]() {OnPlayerLeft(); };
	client->onGameStarted = [this](int selfId, float positions[PLAYERS_NUM][2]) {OnGameStarted(selfId, positions); };

	client->onDirectionChanged = [this](int id, Vector newDir, Vector pos) {
		printf("Direction changed of player %d\n", id);
		if (game == NULL || !game->IsRunning())
			return;

		GameObject* player = game->GetPlayer(id);
		if (player == NULL || player == game->GetControlledPlayer())
			return;

		game->InvokeOnNextFrame([player, newDir, pos]() {
			player->SetPosition(pos);
			player->FindComponent<ConstantMover>()->SetDirection(newDir); 
			});
	};/*
	client->onAimChanged = [this](int id, double rotation) {
		printf("Aiming changed of player %d\n", id);
		if (game == NULL || !game->IsRunning())
			return;

		GameObject* player = game->GetPlayer(id);
		if (player == NULL || player == game->GetControlledPlayer())
			return;

		game->Invoke([player, rotation]() { player->SetRotation(rotation); });
	};*/
	client->onShot = [this](int id, double dir, FirearmType wpnType, Vector pos) {
		printf("Player %d shot\n", id);
		if (game == NULL || !game->IsRunning())
			return;

		GameObject* player = game->GetPlayer(id);
		if (player == NULL || player == game->GetControlledPlayer())
			return;

		game->InvokeOnNextFrame([player, dir, wpnType, pos]() {
			player->SetRotation(dir);
			player->SetPosition(pos);
			PlayerEquipment* eq = player->FindComponent<PlayerEquipment>();
			eq->EquipWeapon(wpnType);
			eq->GetCurrentWeapon()->TryShoot();
			});
	};
	client->onWeaponChanged = [this](int id, FirearmType newType) {
		printf("Player %d changed weapon\n", id);
		if (game == NULL || !game->IsRunning())
			return;

		GameObject* player = game->GetPlayer(id);
		if (player == NULL || player == game->GetControlledPlayer())
			return;

		game->InvokeOnNextFrame([player, newType]() { player->FindComponent<PlayerEquipment>()->EquipWeapon(newType); });
	};
	client->onLabChanged = [this](bool* newWalls) {
		printf("Lab changed\n");
		if (game == NULL || !game->IsRunning())
			return;

		LabyrinthSolidifier* lab = game->GetLab();
		game->InvokeOnNextFrame([newWalls, lab]() {lab->SetLab(newWalls); });
	};
	client->onPlayerHurt = [this](int id, int dmg) {
		printf("Player %d hurt by %d hp\n", id, dmg);
		if (game == NULL || !game->IsRunning())
			return;

		GameObject* player = game->GetPlayer(id);
		if (player == NULL)
			return;

		game->InvokeOnNextFrame([player, dmg]() { player->FindComponent<Health>()->Hurt(dmg); });
	};
}

void GameRoom::SubscribeToGame() {
	game->onControlledDirectionChanged = [this](const Vector& newDir) {
		if (client == NULL)
			return;

		GameObject* player = game->GetControlledPlayer();
		client->Send(ClientSinglePlayer::CreateMessageNewDirection(newDir, player->GetPosition()));
	};/*
	game->onControlledAimChanged = [this](double newRot) {
		if (client == NULL)
			return;

		client->Send(Client::CreateMessageChangeOfAimingDirection(newRot));
	};*/
	game->onControlledShot = [this]() {
		if (client == NULL)
			return;

		GameObject* player = game->GetControlledPlayer();
		client->Send(ClientSinglePlayer::CreateMessagePlayerShot(
			player->GetRotation(),
			player->FindComponent<PlayerEquipment>()->GetCurrentWeapon()->GetType(),
			player->GetPosition()
		));
	};
	game->onControlledWeaponChanged = [this](FirearmType newType) {
		if (client == NULL)
			return;

		client->Send(ClientSinglePlayer::CreateMessageWeaponChange(newType));
	};
}
