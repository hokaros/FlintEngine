#pragma once
#include "../FlatEngine/AssetManager.h"


class FakeAssetManager
	: public AssetManager
{

public:
	void SetPrefabToReturn(GameObject* prefab)
	{
		m_PrefabToReturn = prefab;
	}

	virtual GameObject* GetPrefab(const std::string& path)
	{
		return m_PrefabToReturn;
	}

private:
	GameObject* m_PrefabToReturn;
};
