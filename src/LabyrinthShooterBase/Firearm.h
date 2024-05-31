#pragma once
#include <InputController.h>
#include "Bullet.h"
#include "Shared.h"

enum class FirearmType { Basic, Super };

template<>
class SerializableTypeInterface<FirearmType>
{
public:
	static inline void ParseString(const std::string& str, FirearmType& out_value)
	{
		if (str == "Super")
		{
			out_value = FirearmType::Super;
		}
		else
		{
			out_value = FirearmType::Basic;
		}
	}

	static inline std::string ToString(const FirearmType& value)
	{
		switch (value)
		{
		case FirearmType::Super:
			return "Super";

		default:
			return "Basic";
		}
	}

	STI_DEFINE_TYPECODE_GETTER()
};

class Firearm :
	public ObjectComponent
{
	DECLARE_COMPONENT();
public:
	Firearm() = default;
	Firearm(PrefabRef bulletPrefab, float reloadTime, FirearmType type);

	void Update() override;
	// Strzela, jeœli prze³adowano
	bool TryShoot();

	FirearmType GetType() const;

protected:
	PrefabRef m_BulletPrefab;
	DECLARE_FIELD(m_BulletPrefab);

	static void OnPlayerCollision(GameObject& player, int dmg); // Zdarzenie, gdy pocisk trafi w gracza

private:
	float m_ReloadTime = 1.0f;
	DECLARE_FIELD(m_ReloadTime);

	FirearmType m_Type = FirearmType::Basic;
	DECLARE_FIELD(m_Type);

	float timeSinceLastShot = INFINITY;

	bool isReloaded = true; // czy gotowy do strza³u
};

