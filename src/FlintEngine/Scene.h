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
	
	virtual void AddGameObject(std::unique_ptr<GameObject> game_object) override;

	void MoveObjectsFrom(Scene&& other_scene);

	void SetBackgroundColor(const Rgb8& color);
	const Rgb8& GetBackgroundColor() const;

	const ObjectManager& GetObjectManager() const;

private:
	void RenderBackground(SceneRenderer& renderer);

	virtual bool ShouldRender(GameObject* gameObject);

protected:
	ObjectManager m_ObjectManager;
	Rgb8 m_BackgroundColor = Rgb8(200, 200, 200);
};

