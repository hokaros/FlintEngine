#pragma once
#include <Math/Vector.h>
#include <IUpdateable.h>
#include <ComponentFieldChange.h>
#include "ITransformable.h"
#include "ObjectComponent.h"

class Scene;
class IGameObjectContainer;

class SceneKey
{
private:
	SceneKey() = default;

	friend Scene;
};

enum class GameObjectType
{
	GameObject,
	PrefabInstance
};