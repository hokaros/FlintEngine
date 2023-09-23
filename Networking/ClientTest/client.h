#pragma once
#include "../connection/ConnectionHandler.h"
#include "../connection/Message.h"

#define PLAYERS_NUM 2


class Client
{
public:
	void(*onMouseLocked)(int duration);
public:
	Client(const char* address, int port);
	void Connect();
	void SendKeys(char* keys, size_t count);
	void Disconnect();
	bool IsConnected();
	bool Send(Message<WildMessage>& msg);
	void GameProtocol();
	Message<WildMessage> CreateMessagePlayerDeath(int id);
	Message<WildMessage> CreateMessageWallDestruction(int x, int y);
	Message<WildMessage> CreateMessageLabirynthChange(bool* change, int size);
	//Message<WildMessage> CreateMessagePosition(Vector pos, int id);
	//Message<WildMessage> CreateMessageNewDirection(Vector direction, int id);
	//Message<WildMessage> CreateMessageWeaponChange(WeaponType type, int id);
	Message<WildMessage> CreateMessageChangeOfAimingDirection(float aimDir, int id);
	Message<WildMessage> CreateMessageJoinRequest();
	~Client();
private:
	asio::io_context context;
	asio::io_context::work idleWork;
	asio::ip::tcp::endpoint endpoint;
	ConnectionHandler<WildMessage>* connectionHandler;

	std::thread* contextThread;

	bool disconnected = true;

	void OnMessageReceived(const Message<WildMessage>& msg);
};

