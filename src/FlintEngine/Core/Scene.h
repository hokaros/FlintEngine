#pragma once
#include <Core/GameObject.h>
#include "SceneRenderer.h"
#include "ObjectManager.h"

class Scene
	: public IGameObjectContainer
{
public:
	virtual void OnLoaded();
	virtual void Update();
	virtual void Render(SceneRenderer& renderer);
	void ResetScene();
	void PostFrame();

	IGameObject* FindGameObjectByName(const std::string& name) const;

	using GameObjectsT = std::list<std::unique_ptr<IGameObject>>;
	GameObjectsT::iterator BeginRootGameObjects();
	GameObjectsT::iterator EndRootGameObjects();
	
	virtual void AddGameObject(std::unique_ptr<IGameObject> game_object) override;

	void MoveObjectsFrom(Scene&& other_scene);

	void SetBackgroundColor(const Rgb8& color);
	const Rgb8& GetBackgroundColor() const;

	const ObjectManager& GetObjectManager() const;

private:
	void RenderBackground(SceneRenderer& renderer);

protected:
	ObjectManager m_ObjectManager;
	Rgb8 m_BackgroundColor = Rgb8(200, 200, 200);
};

