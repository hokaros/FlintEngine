#pragma once
#include "ConnectionHandler.h"
#include "Message.h"
#include "GameInfoReceiver.h"
#include "../LabyrinthShooter/Shared.h"
#include "../LabyrinthShooter/Vector.h"
#include "../LabyrinthShooter/Labirynt.h"

using std::function;


class Client : public GameInfoReceiver
{
public:
	function<void()> onJoinAccepted;
	function<void()> onJoinDenied;
	function<void()> onPlayerJoined;
	function<void()> onPlayerLeft;
	function<void(int id, int dmg)> onPlayerHurt;
	function<void(int id)> onPlayerDead;
	function<void(bool*)> onLabChanged;
public:
	Client(const char* address, int port);
	void Connect();
	void SendKeys(char* keys, size_t count);
	void Disconnect();
	bool IsConnected();
	bool Send(const Message<WildMessage>& msg);
	void GameProtocol();
	static Message<WildMessage> CreateMessageWallDestruction(int x, int y);
	//Message<WildMessage> CreateMessagePosition(Vector pos, int id);
	static Message<WildMessage> CreateMessageNewDirection(Vector direction, Vector position);
	static Message<WildMessage> CreateMessageWeaponChange(FirearmType type);
	static Message<WildMessage> CreateMessageChangeOfAimingDirection(double aimDir);
	static Message<WildMessage> CreateMessageJoinRequest();
	static Message<WildMessage> CreateMessagePlayerShot(double aimDir, FirearmType wpnType, Vector sourcePos);

	~Client();
protected:
	virtual void OnMessageReceived(const Message<WildMessage>& msg);

private:
	asio::io_context context;
	asio::io_context::work idleWork;
	asio::ip::tcp::endpoint endpoint;
	ConnectionHandler<WildMessage>* connectionHandler;

	std::thread* contextThread;

	bool disconnected = true;

private:
	void ReceiveMessageLabChanged(Message<WildMessage>& msg);
};

