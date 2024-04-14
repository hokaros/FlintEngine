#pragma once

class ISaveable
{
public:
	virtual void Save() = 0;

	virtual bool HasUnsavedChanges() const = 0;
	virtual void OnUnsavedChange() = 0;

	virtual ~ISaveable() = default;
};