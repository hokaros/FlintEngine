#pragma once
#include <Math/Vector.h>
#include <IUpdateable.h>
#include "ITransformable.h"

class Scene;
class IGameObjectContainer;

class SceneKey
{
private:
	SceneKey() = default;

	friend Scene;
};

class IGameObject
{
public:
	virtual const std::string& GetName() const = 0;
	virtual void SetName(const std::string& name) = 0;

	virtual IGameObject* GetParent() const = 0;
	virtual void SetParent(IGameObject* parent) = 0;

	virtual const std::vector<std::unique_ptr<IGameObject>>& GetChildren() const = 0;
	virtual void MoveChild(IGameObject* child, IGameObjectContainer& new_container) = 0;

	virtual void SetEnabled(bool enabled) = 0;

	virtual void SetScene(Scene* scene, SceneKey) = 0;

	virtual std::unique_ptr<IGameObject> Copy() const = 0;

	virtual IUpdateable& GetUpdateable() = 0;
	virtual const IUpdateable& GetUpdateable() const = 0;
	virtual ITransformable& GetTransformable() = 0;
	virtual const ITransformable& GetTransformable() const = 0;
};