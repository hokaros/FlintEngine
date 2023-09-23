#include "Server.h"

Server::Server()
	: idleWork(context),
	endpoint(asio::ip::tcp::v4(), PORT_DEFAULT), acceptor(context, endpoint),
	contextThread(NULL) {
	//	Inicjacja tablicy graczy
	for (int i = 0; i < PLAYERS_NUM; i++) players[i] = NULL;
}

Server::~Server() {
	Stop();
	delete contextThread;

	for (auto connection : connections) {
		delete connection;
	}
}

void Server::Listen() {
	acceptor.listen();
	contextThread = new std::thread([&]() {context.run(); });

	WaitForClientConnection();
}

void Server::WaitForClientConnection() {
	acceptor.async_accept([&](asio::error_code ec, asio::ip::tcp::socket socket) {
		if (!ec) {
			ConnectionHandler<WildMessage>* newConnection = new ConnectionHandler<WildMessage>(++lastClientId, context, std::move(socket));
			SaveNewConnection(newConnection);
		}

		WaitForClientConnection();
		});
}

void Server::Stop() {
	context.stop();
	if (contextThread != NULL && contextThread->joinable()) {
		contextThread->join();
		delete contextThread;
		contextThread = NULL;
	}
}

void Server::OnMessageReceived(int clientId, const Message<WildMessage>& message) {
	WildMessage msgType = message.header.id;
	Message<WildMessage> msg = message;

	switch (msgType)
	{
	case WildMessage::WRITE:
		for (int i = 0; i < 3; i++)
		{
			bool from_msg;
			msg >> from_msg;
			std::cout << from_msg << "\n";
		}
		break;
	case WildMessage::WALL_DESTRUCTION:
		break;
	case WildMessage::LABIRYNTH_CHANGE:
		break;
	case WildMessage::END_OF_GAME:
		break;
	case WildMessage::POSITION:
		break;

	case WildMessage::NEW_DIRECTION: {
		// Doklejenie id
		msg << clientIdIndexMap[clientId];
		// Rozes³anie
		MessageAllClients(msg);
		// Przetworzenie
		ReceiveMessageNewDirection(msg);
		break;
	}
	case WildMessage::WEAPON_CHANGE: {
		// Doklejenie id
		msg << clientIdIndexMap[clientId];
		// Rozes³anie
		MessageAllClients(msg);
		// Przetworzenie
		ReceiveMessageWeaponChange(msg);
		break;
	}
	case WildMessage::CHANGE_OF_AIMING_DIRECTION: {
		// Doklejenie id
		msg << clientIdIndexMap[clientId];
		// Rozes³anie
		MessageAllClients(msg);
		// Przetworzenie
		ReceiveMessageAimChange(msg);
		break;
	}
	case WildMessage::SHOT: {
		// Doklejenie id
		msg << clientIdIndexMap[clientId];
		// Rozes³anie
		MessageAllClients(msg);
		// Przetworzenie
		ReceiveMessageShot(msg);
		break;
	}
	case WildMessage::PLAYER_DEATH:
		// Doklejenie id
		msg << clientIdIndexMap[clientId];
		// Rozes³anie
		MessageAllClients(msg);
		break;
	case WildMessage::JOIN_REQUEST:
	{
		//	Dodanie gracza do tablicy
		ConnectionHandler<WildMessage>* connection = clientIdMap[clientId];
		bool joined = TryAddPlayer(connection, clientId);

		Message<WildMessage> msg;

		if (joined) msg.header.id = WildMessage::JOIN_ACCEPT;
		else msg.header.id = WildMessage::JOIN_DENIED;

		connection->WriteMessage(msg);

		if (ArePlayersReady()) InitGame();

		break;
	}
	case WildMessage::JOIN_ACCEPT:
		break;
	case WildMessage::JOIN_DENIED:
		break;
	case WildMessage::PLAYER_JOINED:
		break;
	case WildMessage::PLAYER_LEFT:
		break;
	case WildMessage::GAME_STARTED:
		break;
	default: break;
	}
}

void Server::SaveNewConnection(ConnectionHandler<WildMessage>* newConnection)
{
	SubscribeToConnectionEvents(newConnection);

	connections.push_back(newConnection);
	//	Dodanie do slownika powiazania id z ConnectionHandler
	clientIdMap[lastClientId] = newConnection;

	newConnection->OpenInput();

	if (onClientConnected)
		onClientConnected(lastClientId);
}

void Server::SubscribeToConnectionEvents(ConnectionHandler<WildMessage>* connection)
{
	connection->onMessageReceived =
		[this](int clientId, const Message<WildMessage>& msg) {
		OnMessageReceived(clientId, msg);
	};
	connection->onDisconnected = [this](int clientId) {
		OnClientDisconnected(clientId);
	};
}

void Server::OnClientDisconnected(int clientId)
{
	RemovePlayer(clientId);

	if (onClientDisconnected)
		onClientDisconnected(clientId);
}

bool Server::ArePlayersReady()
{
	bool ready = true;
	for (int i = 0; i < PLAYERS_NUM; i++)
	{
		if (players[i] == NULL)
		{
			return false;
		}
	}
	return true;
}

void Server::InitGame()
{
	PlayerPositionsGenerator positionsGenerator;
	positionsGenerator.Generate(PLAYERS_NUM, WIDTH, HEIGHT, MAP_START_X, MAP_START_Y);

	for (int i = 0; i < PLAYERS_NUM; i++)
	{
		Message<WildMessage> message = CreateMessageGameInit(i, positionsGenerator.GetPlayerPositions());
		players[i]->WriteMessage(message);

		if (i == 0)
			ReceiveMessageGameStarted(message); // przetworzenie przez serwer
	}
}

bool Server::TryAddPlayer(ConnectionHandler<WildMessage>* connection, int clientId)
{
	for (int i = 0; i < PLAYERS_NUM; i++)
	{
		if (players[i] == NULL)
		{
			players[i] = connection;
			clientIdIndexMap[clientId] = i;
			return true; // Successfully added
		}
	}

	return false;
}

void Server::RemovePlayer(int clientId)
{
	int playerIndex = clientIdIndexMap[clientId];
	players[playerIndex] = NULL;
}

void Server::MessageAllClients(const Message<WildMessage>& message) {

	for (int i = 0; i < PLAYERS_NUM; i++) {
		if (players[i] != nullptr) {
			players[i]->WriteMessage(message);
		}
	}
}

//	Creating message functions
Message<WildMessage> Server::CreateMessageGameInit(int id, const std::vector<Vector> playerPositions) {

	Message<WildMessage> message;
	message.header.id = WildMessage::GAME_STARTED;

	for (int i = PLAYERS_NUM - 1; i >= 0; i--) {
		message << playerPositions[i].y;
		message << playerPositions[i].x;
	}

	message << id;
	return message;
};

Message<WildMessage> Server::CreateMessagePlayerDeath(int id) {

	Message<WildMessage> message;
	message.header.id = WildMessage::PLAYER_DEATH;
	message << id;
	return message;
};

Message<WildMessage> Server::CreateMessageLabirynthChange(bool* newWalls) {
	Message<WildMessage> message;
	message.header.id = WildMessage::LABIRYNTH_CHANGE;

	int wallsNum = Labirynt::MemorySize(LAB_X, LAB_Y);

	for (int i = wallsNum - 1; i >= 0; i--) {
		message << newWalls[i];
	}

	return message;
}

Message<WildMessage> Server::CreateMessagePlayerHurt(int id, int dmg) {
	Message<WildMessage> message;
	message.header.id = WildMessage::PLAYER_HURT;

	message << dmg << id;

	return message;
}
