#include "GameInfoReceiver.h"

void GameInfoReceiver::ReceiveMessageGameStarted(Message<WildMessage>& msg) {
	std::cout << "Game Started\n";
	int id;
	float positions[PLAYERS_NUM][2];

	msg >> id;
	std::cout << "Id: " << id << "\n";
	for (int i = 0; i < PLAYERS_NUM; i++)
	{
		msg >> positions[i][0];
		msg >> positions[i][1];
		std::cout << "Position " << i << ": " << positions[i][0] << ' ' << positions[i][1] << "\n";
	}

	if (onGameStarted)
		onGameStarted(id, positions);
}

void GameInfoReceiver::ReceiveMessageNewDirection(Message<WildMessage>& msg) {
	return;
	// Odebranie kierunku, w którym zacz¹³ siê poruszaæ gracz i jego id
	int id;
	Vector dir, pos;
	msg >> id >> dir >> pos;

	if (onDirectionChanged)
		onDirectionChanged(id, dir, pos);
}

void GameInfoReceiver::ReceiveMessageWeaponChange(Message<WildMessage>& msg) {
	return;
	int id;
	FirearmType newType;

	msg >> id >> newType;

	if (onWeaponChanged)
		onWeaponChanged(id, newType);
}

void GameInfoReceiver::ReceiveMessageAimChange(Message<WildMessage>& msg) {
	return;
	int id;
	double rotation;
	msg >> id >> rotation;

	if (onAimChanged)
		onAimChanged(id, rotation);
}

void GameInfoReceiver::ReceiveMessageShot(Message<WildMessage>& msg) {
	return;
	int id;
	double rot;
	FirearmType wpnType;
	Vector srcPos;
	msg >> id >> rot >> wpnType >> srcPos;

	if (onShot)
		onShot(id, rot, wpnType, srcPos);
}