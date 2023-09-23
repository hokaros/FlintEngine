#include "Client.h"

Client::Client(const char* address, int port)
	: endpoint(asio::ip::make_address(address), port), connectionHandler(NULL),
	contextThread(NULL), idleWork(context) {
	
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

bool Client::Send(const Message<WildMessage>& msg)
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

	int id;

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
	case WildMessage::WALL_DESTRUCTION:
		break;
	case WildMessage::LABIRYNTH_CHANGE:
		ReceiveMessageLabChanged(msg);
		break;
	case WildMessage::END_OF_GAME:
		break;
	case WildMessage::POSITION:
		break;
	case WildMessage::PLAYER_HURT: {
		int dmg;
		msg >> id >> dmg;

		if (onPlayerHurt)
			onPlayerHurt(id, dmg);
		break;
	}
	case WildMessage::PLAYER_DEATH:
		msg >> id;

		if (onPlayerDead)
			onPlayerDead(id);
		break;
	case WildMessage::NEW_DIRECTION:
		ReceiveMessageNewDirection(msg);
		break;
	case WildMessage::WEAPON_CHANGE:
		ReceiveMessageWeaponChange(msg);
		break;
	case WildMessage::CHANGE_OF_AIMING_DIRECTION:
		ReceiveMessageAimChange(msg);
		break;
	case WildMessage::SHOT:
		ReceiveMessageShot(msg);
		break;
	case WildMessage::JOIN_REQUEST:
		break;
	case WildMessage::JOIN_ACCEPT:
		std::cout << "Joined\n";

		if (onJoinAccepted)
			onJoinAccepted();
		break;
	case WildMessage::JOIN_DENIED:
		std::cout << "Waiting\n";

		if (onJoinDenied)
			onJoinDenied();
		break;
	case WildMessage::PLAYER_JOINED:
		if (onPlayerJoined)
			onPlayerJoined();
		break;
	case WildMessage::PLAYER_LEFT:
		if (onPlayerLeft)
			onPlayerLeft();
		break;
	case WildMessage::GAME_STARTED:
		ReceiveMessageGameStarted(msg);
		break;
	default: break;
	}
}
//Zmiany
Message<WildMessage> Client::CreateMessageWallDestruction(int x, int y) {

	Message<WildMessage> message;
	message.header.id = WildMessage::WALL_DESTRUCTION;
	message << x;
	message << y;
	return message;
};
/*Message<WildMessage> Client::CreateMessagePosition(Vector pos, int id) {

	Message<WildMessage> message;
	message.header.id = WildMessage::POSITION;
	message << id;
	message << pos;
	return message;
};*/
Message<WildMessage> Client::CreateMessageNewDirection(Vector direction, Vector position) {

	Message<WildMessage> message;
	message.header.id = WildMessage::NEW_DIRECTION;
	message << position << direction;
	return message;
};
Message<WildMessage> Client::CreateMessageWeaponChange(FirearmType type) {

	Message<WildMessage> message;
	message.header.id = WildMessage::WEAPON_CHANGE;
	message << type;
	return message;
};
Message<WildMessage> Client::CreateMessageChangeOfAimingDirection(double aimDir) {

	Message<WildMessage> message;
	message.header.id = WildMessage::CHANGE_OF_AIMING_DIRECTION;
	message << aimDir;
	return message;
};
Message<WildMessage> Client::CreateMessageJoinRequest() {

	Message<WildMessage> message;
	message.header.id = WildMessage::JOIN_REQUEST;
	return message;
};
Message<WildMessage> Client::CreateMessagePlayerShot(double aimDir, FirearmType wpnType, Vector sourcePos) {

	Message<WildMessage> message;
	message.header.id = WildMessage::SHOT;
	message << sourcePos << wpnType << aimDir;
	return message;
}


// Odbieranie wiadomoœci
void Client::ReceiveMessageLabChanged(Message<WildMessage>& msg) {
	int wallsNum = Labirynt::MemorySize(LAB_X, LAB_Y);
	bool* walls = new bool[wallsNum];

	for (int i = 0; i < wallsNum; i++) msg >> walls[i];

	if (onLabChanged)
		onLabChanged(walls);
}