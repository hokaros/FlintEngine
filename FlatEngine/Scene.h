#pragma once
#include "GameObject.h"
#include "SceneRenderer.h"
#include "ObjectManager.h"

class Scene
{
public:
	virtual void Update();
	virtual void Render();
	void PostFrame();
	
	void AddGameObject(std::unique_ptr<GameObject> game_object);
	void SetBackgroundColor(const Rgb8& color);

private:
	void RenderBackground();

	virtual bool ShouldRender(GameObject* gameObject);

protected:
	ObjectManager m_ObjectManager;
	Rgb8 m_BackgroundColor = Rgb8(200, 200, 200);
};

