#pragma once
#include <list>

#include "../FlatEngine/Timer.h"
#include "../FlatEngine/Draw.h"
#include "../FlatEngine/InputController.h"
#include "../FlatEngine/Vector.h"
#include "../FlatEngine/GameObject.h"
#include "../FlatEngine/RectangleRenderer.h"
#include "../FlatEngine/SpriteRenderer.h"
#include "PlayerController.h"
#include "Cage.h"
#include "../FlatEngine/Window.h"
#include "LabyrinthSolidifier.h"
#include "../FlatEngine/ObjectManager.h"
#include "PowerBullet.h"
#include "PlayerEquipment.h"
#include "Firearm.h"
#include "Health.h"

// Czas, co jaki zmienia siê labirynt
#define LAB_TIME 2

#define BULLET_BASIC_SPEED 1000
#define BULLET_BASIC_DAMAGE 1
#define BULLET_SUPER_SPEED 2000
#define BULLET_SUPER_DAMAGE 2
#define WPN_BASIC_RELOAD 0.2
#define WPN_SUPER_RELOAD 2

#define MAX_HEALTH 3
#define PLAYER_SPEED 300.0

#define PLAYER_SIGHT 400


struct GameBitmaps {
public:
	SDL_Surface* playerBmp;
	SDL_Surface* wpnBasicBmp;
	SDL_Surface* wpnSuperBmp;
	SDL_Surface* heartBmp;

public:
	GameBitmaps();
	~GameBitmaps();

	bool IsOk() const;

private:
	bool bitmapsOk = true;
};


struct GameStartInfo {
public:
	GameStartInfo(const Vector& player_position);

	const Vector& GetPlayerPosition() const;

private:
	Vector m_PlayerPosition;
};


class Game
{
public:
	function<void(const Vector&)> onControlledDirectionChanged;
	function<void(double)> onControlledAimChanged;
	function<void()> onControlledShot;
	function<void(FirearmType)> onControlledWeaponChanged;
	function<void(bool*)> onLabChanged;
	function<void(int id, int dmg)> onPlayerHurt;

public:
	Game(Window* window, GameStartInfo&& gameInfo);

	// G³ówna pêtla gry. Zwraca fa³sz, jeœli w trakcie u¿ytkownik zamknie okno
	bool Run();
	bool IsRunning();

	// Usuwa wszystkie obiekty
	void Clear();

	void InvokeOnNextFrame(function<void()> fun);

	GameObject* GetPlayer();

	LabyrinthSolidifier* GetLab() const;

private:
	Window* window = NULL;
	Timer timer;
	ObjectManager objectManager;
	GameObject* m_Player = NULL;
	LabyrinthSolidifier* lab;

	GameStartInfo startInfo;
	bool isRunning = false;

	GameBitmaps* bitmaps = NULL;

	BMPStats* healthStats = NULL;

	GameObject basicBullet; // TODO: przenieœæ prefaby do osobnej struktury
	GameObject superBullet;

	// Lista funkcji do wykonania w najbli¿szej klatce
	std::list<function<void()>> invokes;

	std::mutex invokesMutex;
	std::mutex playersMutex;
	std::mutex metadataMutex;

private:
	void LoadStartingObjects();
	void SetRunning(bool running);

	GameObject* CreatePlayer(const Vector& position);

	void InvokePostponed();

	void OnControlledDirectionChanged(const Vector& newDir);
	void OnBulletPlayerHit(GameObject& player, int dmg);

	void Render();

};