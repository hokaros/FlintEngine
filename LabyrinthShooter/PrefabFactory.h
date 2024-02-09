#pragma once
#include <map>
#include "../FlatEngine/GameObject.h"

#define BULLET_BASIC_SPEED 1000
#define BULLET_BASIC_DAMAGE 1
#define BULLET_SUPER_SPEED 2000
#define BULLET_SUPER_DAMAGE 2

#define WPN_BASIC_RELOAD 0.2
#define WPN_SUPER_RELOAD 2

class PrefabFactory
{
public:
	enum class EPrefabId
	{
		BasicBullet,
		SuperBullet,
		BasicFirearm,
		SuperFirearm,
		Player
	};

public:
	PrefabFactory();

	const GameObject& GetPrefab(EPrefabId prefab_id) const;

private:
	void CreateBasicBulletPrefab();
	void CreateSuperBulletPrefab();
	void CreateBasicFirearmPrefab();
	void CreateSuperFirearmPrefab();
	void CreatePlayerPrefab();

	void InsertPrefab(EPrefabId prefab_id, std::unique_ptr<GameObject> prefab);

private:
	std::map<EPrefabId, std::unique_ptr<GameObject>> m_PrefabDict;
};

