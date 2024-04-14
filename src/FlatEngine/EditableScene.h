#pragma once
#include "Scene.h"
#include "IEditableGameObject.h"
#include "IHierarchyEditable.h"

class EditableScene
	: public IHierarchyEditable
{
public:
	void Render(SceneRenderer& renderer);
	
	std::unique_ptr<Scene> CreateRuntimeObject() const;

	void AddRootObject(std::unique_ptr<IEditableGameObject> game_object);
	const std::vector<std::unique_ptr<IEditableGameObject>>& GetRootObjects() const;

	void SetBackgroundColor(const Rgb8& color);
	const Rgb8& GetBackgroundColor() const;

private:
	void CopyObjectsToScene(Scene& scene) const;

	// IHierarchyEditable
	virtual const std::vector<std::unique_ptr<IEditableGameObject>>& GetSubRootObjects() const override;
	virtual void AddChild(std::unique_ptr<IEditableGameObject> child) override;
	virtual void DeleteChild(IEditableGameObject& child) override;

	virtual const char* GetName() const override;

private:
	std::vector<std::unique_ptr<IEditableGameObject>> m_RootObjects;
	Rgb8 m_BackgroundColor = Rgb8(200, 200, 200);

	Scene m_ResultScene;
};

