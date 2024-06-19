#pragma once

class IEditableGameObject;

class IHierarchyEditable
{
public:
	virtual const std::vector<std::unique_ptr<IGameObject>>& GetSubRootObjects() const = 0;
	virtual void AddChild(std::unique_ptr<IEditableGameObject> child) = 0;
	virtual void DeleteChild(IEditableGameObject& child) = 0;

	virtual const char* GetName() const = 0;

	virtual ~IHierarchyEditable() = default;
};