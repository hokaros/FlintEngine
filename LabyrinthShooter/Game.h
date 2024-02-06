#pragma once
#include <list>

#include "../FlatEngine/RectangleRenderer.h"
#include "../FlatEngine/SpriteRenderer.h"
#include "../LabyrinthShooterBase/PlayerController.h"
#include "../LabyrinthShooterBase/PowerBullet.h"
#include "../LabyrinthShooterBase/PlayerEquipment.h"
#include "../LabyrinthShooterBase/Firearm.h"
#include "../LabyrinthShooterBase/Health.h"
#include "LabyrinthSolidifier.h"
#include "PrefabFactory.h"

#include "../FlatEngine/GameBase.h"
#include "../FlatEngine/AssetManager.h"

// Czas, co jaki zmienia si� labirynt
#define LAB_TIME 2

#define MAX_HEALTH 3
#define PLAYER_SPEED 300.0

#define PLAYER_SIGHT 400


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
	Game(Window* window, GameStartInfo&& gameInfo);

	GameObject* GetPlayer();

	LabyrinthSolidifier* GetLab() const;

private:
	GameObject* m_Player = NULL;
	LabyrinthSolidifier* lab;

	GameStartInfo startInfo;

	AssetManager m_AssetManager;

	std::unique_ptr<BMPStats> m_HealthStats = nullptr;

	PrefabFactory m_PrefabFactory;
	std::mutex playersMutex;

	static Vector s_BasicBulletSize;
	static Vector s_SuperBulletSize;

private:
	void LoadStartingObjects();

	GameObject* CreatePlayer(const Vector& position);

	void OnBulletPlayerHit(GameObject& player, int dmg);

	virtual void PreRun() override;
	virtual void PostRun() override;
	virtual void PrePresent() override;
	virtual void PostObjectsUpdate() override;
	virtual bool ShouldRender(GameObject* gameObject) override;
};