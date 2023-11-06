#pragma once
#include <list>

#include "../FlatEngine/RectangleRenderer.h"
#include "../FlatEngine/SpriteRenderer.h"
#include "PlayerController.h"
#include "LabyrinthSolidifier.h"
#include "PowerBullet.h"
#include "PlayerEquipment.h"
#include "Firearm.h"
#include "Health.h"
#include "PrefabFactory.h"

#include "../FlatEngine/GameBase.h"

// Czas, co jaki zmienia siê labirynt
#define LAB_TIME 2

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
	: public GameBase
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

	GameObject* GetPlayer();

	LabyrinthSolidifier* GetLab() const;

private:
	GameObject* m_Player = NULL;
	LabyrinthSolidifier* lab;

	GameStartInfo startInfo;

	GameBitmaps* bitmaps = NULL;

	BMPStats* healthStats = NULL;

	PrefabFactory m_PrefabFactory;
	std::mutex playersMutex;

	static Vector s_BasicBulletSize;
	static Vector s_SuperBulletSize;

private:
	void LoadStartingObjects();

	GameObject* CreatePlayer(const Vector& position);

	void OnControlledDirectionChanged(const Vector& newDir);
	void OnBulletPlayerHit(GameObject& player, int dmg);

	virtual void PreRun() override;
	virtual void PostRun() override;
	virtual void PrePresent() override;
	virtual void PostObjectsUpdate() override;
	virtual bool ShouldRender(GameObject* gameObject) override;
};