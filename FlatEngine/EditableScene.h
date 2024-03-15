#pragma once
#include "Scene.h"
#include "IEditableGameObject.h"

class EditableScene
{
public:
	std::unique_ptr<Scene> CreateRuntimeObject() const;

	void AddRootObject(std::unique_ptr<IEditableGameObject> game_object);
	const std::vector<std::unique_ptr<IEditableGameObject>>& GetRootObjects() const;

	void SetBackgroundColor(const Rgb8& color);
	const Rgb8& GetBackgroundColor() const;

private:
	void CopyObjectsToScene(Scene& scene) const;

	static void CopyChildrenToRuntimeObject(const IEditableGameObject& src, GameObject& dest);

private:
	std::vector<std::unique_ptr<IEditableGameObject>> m_RootObjects;
	Rgb8 m_BackgroundColor = Rgb8(200, 200, 200);
};

