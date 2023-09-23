#pragma once
#include <thread>
#include <deque>
#include <map>
#include <vector>
#include "../LabyrinthShooter/Shared.h"
#include "../LabyrinthShooter/Labirynt.h"
#include "../LabyrinthShooter/PlayerPositionsGenerator.h"

#include "ConnectionHandler.h"
#include "Message.h"
#include "GameInfoReceiver.h"

#define PORT_DEFAULT 80

using std::function;


class Server : public GameInfoReceiver
{
public:
	// Zdarzenia
	void (*onClientConnected)(int id);
	void (*onClientDisconnected)(int id);
	void (*onMessageReceived) (int clientId, const std::string& msg);

public:
	Server();
	void Listen();

	void MessageAllClients(const Message<WildMessage>& message);

	void Stop();
	~Server();

	Message<WildMessage> CreateMessageGameInit(int id, const std::vector<Vector> playerPositions);

	static Message<WildMessage> CreateMessagePlayerDeath(int id);
	static Message<WildMessage> CreateMessageLabirynthChange(bool* newWalls);
	static Message<WildMessage> CreateMessagePlayerHurt(int id, int dmg);

protected:
	void OnMessageReceived(int clientId, const Message<WildMessage>& message);

private:
	void SaveNewConnection(ConnectionHandler<WildMessage>* newConnection);
	void SubscribeToConnectionEvents(ConnectionHandler<WildMessage>* connection);

	void OnClientDisconnected(int clientId);

	bool ArePlayersReady();
	void InitGame();

	bool TryAddPlayer(ConnectionHandler<WildMessage>* connection, int clientId);
	void RemovePlayer(int clientId);

	int lastClientId = 0;

	asio::ip::tcp::endpoint endpoint;
	asio::io_context context;
	asio::io_context::work idleWork;
	asio::ip::tcp::acceptor acceptor;

	std::deque<ConnectionHandler<WildMessage>*> connections;
	std::map<int, ConnectionHandler<WildMessage>*> clientIdMap;
	std::map<int, int> clientIdIndexMap;
	std::thread* contextThread;

	ConnectionHandler<WildMessage>* players[PLAYERS_NUM];
private:
	void WaitForClientConnection();
};

