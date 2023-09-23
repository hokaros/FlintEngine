#pragma once
#include "ConnectionHandler.h"
#include "Message.h"
#include "../LabyrinthShooter/Shared.h"
#include "../LabyrinthShooter/Vector.h"
#include "../LabyrinthShooter/Shared.h"

using std::function;

class GameInfoReceiver
{
public:
	function<void(int id, float positions[PLAYERS_NUM][2])> onGameStarted;
	function<void(int id, Vector newDir, Vector pos)> onDirectionChanged;
	function<void(int id, FirearmType newType)> onWeaponChanged;
	function<void(int id, double rotation)> onAimChanged;
	function<void(int id, double dir, FirearmType, Vector pos)> onShot;

protected:
	void ReceiveMessageGameStarted(Message<WildMessage>& msg);
	void ReceiveMessageNewDirection(Message<WildMessage>& msg);
	void ReceiveMessageWeaponChange(Message<WildMessage>& msg);
	void ReceiveMessageAimChange(Message<WildMessage>& msg);
	void ReceiveMessageShot(Message<WildMessage>& msg);
};

