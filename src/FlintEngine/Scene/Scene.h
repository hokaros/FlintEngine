#pragma once
#include <Core/GameObject.h>
#include <Rendering/SceneRenderer.h>
#include "ObjectManager.h"
#include <Navigation/Navmesh.h>
#include <Navigation/NavmeshGenerator.h>
#include <Scene/CameraManager.h>

class Scene
	: public IGameObjectContainer
{
public:
	Scene() = default;
	Scene(const Scene& other);

	virtual void OnLoaded();
	virtual void Update();
	virtual void Render(SceneRenderer& renderer);
	void ResetScene();
	void PostFrame();

	GameObject* FindGameObjectByName(const std::string& name) const;

	using GameObjectsT = std::vector<std::unique_ptr<GameObject>>;
	GameObjectsT::iterator BeginRootGameObjects();
	GameObjectsT::iterator EndRootGameObjects();
	
	virtual void AddGameObject(std::unique_ptr<GameObject> game_object) override;
	virtual const std::vector<std::unique_ptr<GameObject>>& GetGameObjects() const override;
	void RemoveGameObject(GameObject& game_object);

	void MoveObjectsFrom(Scene&& other_scene);
	void CopyObjectsFrom(const Scene& other);

	void SetBackgroundColor(const Rgb8& color);
	const Rgb8& GetBackgroundColor() const;

	const ObjectManager& GetObjectManager() const;
	scene::CameraManager& GetCameraManager();

	void RegenerateNavmesh();
	void SetNavmesh(const Navigation::Navmesh& other);
	const Navigation::Navmesh& GetNavmesh() const;
	const Navigation::NavmeshGenerationParams& GetNavmeshGenerationParams() const;
	Navigation::NavmeshGenerationParams& GetNavmeshGenerationParams();
	void SetNavmeshGenerationParams(const Navigation::NavmeshGenerationParams& params);

private:
	void RenderBackground(SceneRenderer& renderer);
	void SetViewportBasedOnCamera(SceneRenderer& renderer);

protected:
	ObjectManager m_ObjectManager;
	Rgb8 m_BackgroundColor = Rgb8(200, 200, 200);

	Navigation::NavmeshGenerationParams m_NavmeshGenerationParams;
	Navigation::Navmesh m_Navmesh;

	scene::CameraManager m_CameraManager;
};

