#pragma once
#include <Game.h>
#include <Draw.h>
#include <TextBox.h>
#include <mutex>
#include "ClientSinglePlayer.h"

#define FONTSIZE_HEADER 25
#define FONTSIZE_MEDIUM 14

#define DEFAULT_PORT 80

class GameRoom
{
public:
	GameRoom(Window& window, ClientSinglePlayer* client);

	// G³ówna pêtla poczekalni
	void Enter();
	// Wejœcie do gry
	void StartGame(int selfId, Vector* playerPositions, size_t playerCount);

	int GetPlayerCount();
	void SetPlayerCount(int count);

private:
	Window& window;
	Game* game = NULL; // obecna gra
	Game* nextGame = NULL; // gra przygotowana do uruchomienia

	ClientSinglePlayer* client;

	int playerCount;

	bool startGame = false; // Sygna³ do rozpoczêcia gry
	std::mutex mutex;

private:
	void DrawWaitingRoom();

	// Uruchamia pêtlê gry
	void StartGameServer();
	void RunGame();

	void SetShouldStart(bool shouldStart);
	bool ShouldStart();

	void SetNextGame(Game* newGame);

	void SubscribeToClient();
	void SubscribeToGame();

	void OnPlayerJoined();
	void OnPlayerLeft();
	void OnGameStarted(int selfId, float positions[PLAYERS_NUM][2]);
};
