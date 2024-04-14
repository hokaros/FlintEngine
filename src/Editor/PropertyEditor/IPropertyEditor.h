#pragma once

class IPropertyEditor
{
public:
	virtual void RenderEmbedded() = 0;
	virtual bool HasValidObject() const = 0;

	virtual ~IPropertyEditor() = default;
};