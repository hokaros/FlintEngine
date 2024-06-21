#pragma once

class GameObject;

class IHierarchyEditable
{
public:
	virtual const std::vector<std::unique_ptr<GameObject>>& GetSubRootObjects() const = 0;
	virtual void AddChild(std::unique_ptr<GameObject> child) = 0;
	virtual void DeleteChild(GameObject& child) = 0;

	virtual const char* GetName() const = 0;

	virtual ~IHierarchyEditable() = default;
};