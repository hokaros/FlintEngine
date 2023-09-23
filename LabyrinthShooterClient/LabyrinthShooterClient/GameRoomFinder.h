#pragma once
#include "GameRoom.h"

// Panel, w którym gracz wybiera serwer
class GameRoomFinder
{
public:
	GameRoomFinder(Window& window);
	~GameRoomFinder();

	void SearchLoop();
	void EnterGameRoom();

	GameRoom* GetCurrentRoom();

private:
	Window& window;
	GameRoom* currentRoom = NULL;
	ClientSinglePlayer* client = NULL;

	bool shouldEnter = false;

	std::mutex mutex;

private:
	void Draw(TextBox& textBox);
	TextBox CreateIpTextBox();

	void SetCurrentRoom(GameRoom* newRoom);
	// Konsumuj¹ce sprawdzenie sygna³u do wejœcia do pokoju
	bool ShouldEnter();

	void TryConnect(std::string ip);
	void RealEnterGameRoom();
};
