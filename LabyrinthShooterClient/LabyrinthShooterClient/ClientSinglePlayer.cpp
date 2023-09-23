#include "ClientSinglePlayer.h"
#include <PlayerPositionsGenerator.h>
#include <Shared.h>

ClientSinglePlayer::ClientSinglePlayer(const char* address, int port)
{

}

ClientSinglePlayer::~ClientSinglePlayer() {
	Disconnect();
}

void ClientSinglePlayer::Connect() 
{

}

void ClientSinglePlayer::SendKeys(char* keys, size_t count) {
	// TODO: is this function even needed?
	/*if (connectionHandler == NULL)
		return;

	Message<WildMessage> message;
	message.header.id = WildMessage::KEYS;

	for (size_t i = 0; i < count; i++) {
		message << keys[i];
	}
	connectionHandler->WriteMessage(message);*/
}

void ClientSinglePlayer::Disconnect() 
{

}

bool ClientSinglePlayer::IsConnected() {
	return true;
}

bool ClientSinglePlayer::Send(const Message<WildMessage>& msg)
{
	OnMessageReceived(msg);
	return true;
}

void ClientSinglePlayer::GameProtocol()
{
	Message<WildMessage> msg;
	msg.header.id = WildMessage::JOIN_REQUEST;

	Send(msg);
}


void ClientSinglePlayer::OnMessageReceived(const Message<WildMessage>& message) {
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
	{
		Message<WildMessage> response_msg;
		response_msg.header.id = WildMessage::JOIN_ACCEPT;
		Send(response_msg);
		break;
	}
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
Message<WildMessage> ClientSinglePlayer::CreateMessageWallDestruction(int x, int y) {

	Message<WildMessage> message;
	message.header.id = WildMessage::WALL_DESTRUCTION;
	message << x;
	message << y;
	return message;
};
/*Message<WildMessage> ClientSinglePlayer::CreateMessagePosition(Vector pos, int id) {

	Message<WildMessage> message;
	message.header.id = WildMessage::POSITION;
	message << id;
	message << pos;
	return message;
};*/
Message<WildMessage> ClientSinglePlayer::CreateMessageNewDirection(Vector direction, Vector position) {

	Message<WildMessage> message;
	message.header.id = WildMessage::NEW_DIRECTION;
	message << position << direction;
	return message;
};
Message<WildMessage> ClientSinglePlayer::CreateMessageWeaponChange(FirearmType type) {

	Message<WildMessage> message;
	message.header.id = WildMessage::WEAPON_CHANGE;
	message << type;
	return message;
};
Message<WildMessage> ClientSinglePlayer::CreateMessageChangeOfAimingDirection(double aimDir) {

	Message<WildMessage> message;
	message.header.id = WildMessage::CHANGE_OF_AIMING_DIRECTION;
	message << aimDir;
	return message;
};
Message<WildMessage> ClientSinglePlayer::CreateMessageJoinRequest() {

	Message<WildMessage> message;
	message.header.id = WildMessage::JOIN_REQUEST;
	return message;
};
Message<WildMessage> ClientSinglePlayer::CreateMessagePlayerShot(double aimDir, FirearmType wpnType, Vector sourcePos) {

	Message<WildMessage> message;
	message.header.id = WildMessage::SHOT;
	message << sourcePos << wpnType << aimDir;
	return message;
}


// Odbieranie wiadomoœci
void ClientSinglePlayer::ReceiveMessageLabChanged(Message<WildMessage>& msg) {
	int wallsNum = Labirynt::MemorySize(LAB_X, LAB_Y);
	bool* walls = new bool[wallsNum];

	for (int i = 0; i < wallsNum; i++) msg >> walls[i];

	if (onLabChanged)
		onLabChanged(walls);
}

void ClientSinglePlayer::InitGame()
{
	PlayerPositionsGenerator positionsGenerator;
	positionsGenerator.Generate(PLAYERS_NUM, WIDTH, HEIGHT, MAP_START_X, MAP_START_Y);

	Message<WildMessage> message = CreateMessageGameInit(0, positionsGenerator.GetPlayerPositions());

	ReceiveMessageGameStarted(message);
}

Message<WildMessage> ClientSinglePlayer::CreateMessageGameInit(int id, const std::vector<Vector> playerPositions)
{
	Message<WildMessage> message;
	message.header.id = WildMessage::GAME_STARTED;

	for (int i = PLAYERS_NUM - 1; i >= 0; i--) {
		message << playerPositions[i].y;
		message << playerPositions[i].x;
	}

	message << id;
	return message;
}
