#include "Client.h"

Client::Client(const char* address, int port)
	: endpoint(asio::ip::make_address(address), port), connectionHandler(NULL),
	contextThread(NULL), idleWork(context), onMouseLocked(NULL) {
	
}

Client::~Client() {
	Disconnect();
	delete connectionHandler;
}

void Client::Connect() {
	std::error_code ec;
	contextThread = new std::thread([&]() {context.run(); });

	asio::ip::tcp::socket socket(context);
	socket.connect(endpoint, ec);
	if (ec)
		return; // b³¹d

	connectionHandler = new ConnectionHandler<WildMessage>(0, context, std::move(socket));
	connectionHandler->onMessageReceived = [this](int conId, const Message<WildMessage>& msg) {OnMessageReceived(msg); };
	connectionHandler->OpenInput();

	disconnected = false;
}

void Client::SendKeys(char* keys, size_t count) {
	if (connectionHandler == NULL)
		return;
	
	Message<WildMessage> message;
	message.header.id = WildMessage::KEYS;

	for (size_t i = 0; i < count; i++) {
		message << keys[i];
	}
	connectionHandler->WriteMessage(message);
}

void Client::Disconnect() {
	context.stop();
	if (contextThread != NULL && contextThread->joinable()) {
		contextThread->join();
		delete contextThread;
		contextThread = NULL;
	}
	if (connectionHandler != NULL) {
		delete connectionHandler;
		connectionHandler = NULL;
	}

	disconnected = true;
}

bool Client::IsConnected() {
	return !disconnected;
}

bool Client::Send(Message<WildMessage>& msg)
{
	if (connectionHandler == NULL) {
		return false;
	}

	if (!connectionHandler->IsConnected()) {
		//onClientDisconnected(connection->GetId());
		return false;
	}
	connectionHandler->WriteMessage(msg);
	return true;
}

void Client::GameProtocol()
{
	Message<WildMessage> msg;
	msg.header.id = WildMessage::JOIN_REQUEST;

	Send(msg);
}


void Client::OnMessageReceived(const Message<WildMessage>& message) {
	WildMessage msgType = message.header.id;
	Message<WildMessage> msg = message;

	switch (msgType)
	{
	case WildMessage::WRITE:
		for (int i = 0; i < 3; i++)
		{
			int from_msg;
			msg >> from_msg;
			std::cout << "\n" << from_msg << "\n";
		}
		break;
	case WildMessage::PLAYER_DEATH:
		break;
	case WildMessage::WALL_DESTRUCTION:
		break;
	case WildMessage::LABIRYNTH_CHANGE:
		break;
	case WildMessage::END_OF_GAME:
		break;
	case WildMessage::POSITION:
		break;
	case WildMessage::NEW_DIRECTION:
		break;
	case WildMessage::WEAPON_CHANGE:
		break;
	case WildMessage::CHANGE_OF_AIMING_DIRECTION:
		break;
	case WildMessage::SHOT:
		break;
	case WildMessage::JOIN_REQUEST:
		break;
	case WildMessage::JOIN_ACCEPT:
		std::cout << "Joined\n";
		break;
	case WildMessage::JOIN_DENIED:
		std::cout << "Waiting\n";
		break;
	case WildMessage::PLAYER_JOINED:
		break;
	case WildMessage::PLAYER_LEFT:
		break;
	case WildMessage::GAME_STARTED:
	{
		std::cout << "Game Started\n";
		int id;
		float positions[PLAYERS_NUM][2];

		//	TODO
		int num = 4;

		msg >> id;
		std::cout << "Id: " << id << "\n";
		for (int i = 0; i < PLAYERS_NUM; i++)
		{
			msg >> positions[i][0];
			msg >> positions[i][1];
			std::cout << "Position " << i << ": " << positions[i][0] << ' ' << positions[i][1] << "\n";
		}

		std::cout << "Walls : " ;
		for (int i = 0; i < num; i++)
		{
			bool wall;
			msg >> wall;
			std::cout << wall << ' ';
		}
		std::cout << "\n";
		//
		break;
	}
	default: break;
	}
}
//Zmiany
Message<WildMessage> Client::CreateMessagePlayerDeath(int id) {

	Message<WildMessage> message;
	message.header.id = WildMessage::PLAYER_DEATH;
	message << id;
	return message;
};
Message<WildMessage> Client::CreateMessageWallDestruction(int x, int y) {

	Message<WildMessage> message;
	message.header.id = WildMessage::WALL_DESTRUCTION;
	message << x;
	message << y;
	return message;
};
Message<WildMessage> Client::CreateMessageLabirynthChange(bool* change, int size) {

	Message<WildMessage> message;
	message.header.id = WildMessage::LABIRYNTH_CHANGE;
	for (int i = 0; i < size; i++) {
		message << change[i];
	}
	return message;
};
/*Message<WildMessage> Client::CreateMessagePosition(Vector pos, int id) {

	Message<WildMessage> message;
	message.header.id = WildMessage::POSITION;
	message << id;
	message << pos;
	return message;
};
Message<WildMessage> Client::CreateMessageNewDirection(Vector direction, int id) {

	Message<WildMessage> message;
	message.header.id = WildMessage::NEW_DIRECTION;
	message << id;
	message << direction;
	return message;
};
Message<WildMessage> Client::CreateMessageWeaponChange(WeaponType type, int id) {

	Message<WildMessage> message;
	message.header.id = WildMessage::WEAPON_CHANGE;
	message << id;
	message << type;
	return message;
};*/
Message<WildMessage> Client::CreateMessageChangeOfAimingDirection(float aimDir, int id) {

	Message<WildMessage> message;
	message.header.id = WildMessage::CHANGE_OF_AIMING_DIRECTION;
	message << id;
	message << aimDir;
	return message;
};
Message<WildMessage> Client::CreateMessageJoinRequest() {

	Message<WildMessage> message;
	message.header.id = WildMessage::JOIN_REQUEST;
	return message;
};