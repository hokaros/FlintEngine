#pragma once

class IEditableGameObject;

class IHierarchyEditable
{
public:
	virtual const std::vector<std::unique_ptr<IEditableGameObject>>& GetSubRootObjects() = 0; // Immediate children of the root

	virtual ~IHierarchyEditable() = default;
};