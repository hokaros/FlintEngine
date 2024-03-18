#pragma once
#include "Scene.h"
#include "IEditableGameObject.h"
#include "IHierarchyEditable.h"

class EditableScene
	: public IHierarchyEditable
{
public:
	void Render();
	
	std::unique_ptr<Scene> CreateRuntimeObject() const;

	void AddRootObject(std::unique_ptr<IEditableGameObject> game_object);
	const std::vector<std::unique_ptr<IEditableGameObject>>& GetRootObjects() const;

	void SetBackgroundColor(const Rgb8& color);
	const Rgb8& GetBackgroundColor() const;

private:
	void CopyObjectsToScene(Scene& scene) const;

	// IHierarchyEditable
	virtual const std::vector<std::unique_ptr<IEditableGameObject>>& GetSubRootObjects() override;
	virtual void DeleteChild(IEditableGameObject& child) override;

private:
	std::vector<std::unique_ptr<IEditableGameObject>> m_RootObjects;
	Rgb8 m_BackgroundColor = Rgb8(200, 200, 200);
};

