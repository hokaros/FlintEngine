#pragma once
#include <Core/Scene.h>
#include <Core/GameObject.h>

class EditableScene
{
public:
	void Render(SceneRenderer& renderer);
	
	std::unique_ptr<Scene> CreateRuntimeObject() const;

	void AddRootObject(std::unique_ptr<GameObject> game_object);
	void RemoveRootObject(GameObject& game_object);
	const std::vector<std::unique_ptr<GameObject>>& GetRootObjects() const;

	void SetBackgroundColor(const Rgb8& color);
	const Rgb8& GetBackgroundColor() const;

private:
	void CopyObjectsToScene(Scene& scene) const;

private:
	std::vector<std::unique_ptr<GameObject>> m_RootObjects;
	Rgb8 m_BackgroundColor = Rgb8(200, 200, 200);

	Scene m_ResultScene;
};

