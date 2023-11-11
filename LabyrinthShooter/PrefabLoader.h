#pragma once
#include "../FlatEngine/GameObject.h"

class ComponentDefinitionTest
{
public:
	ComponentDefinitionTest();
};

class PrefabLoader
{
public:
	static std::unique_ptr<GameObject> LoadPrefab(const char* file_path);
private:
	static ComponentDefinitionTest s_Test;
};

