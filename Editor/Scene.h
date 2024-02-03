#pragma once
#include "../FlatEngine/GameObject.h"

// TODO: move to the engine
class Scene
{
public:
	void Render();

private:
	std::vector<GameObject> m_GameObjects;
};

