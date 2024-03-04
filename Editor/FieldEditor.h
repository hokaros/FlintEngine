#pragma once
#include "../FlatEngine/ComponentFieldDefinition.h"
#include "../FlatEngine/imgui/imgui.h"

class FieldEditor
{
public:
	FieldEditor(ObjectComponent& component, const ComponentFieldDefinition& field);

	virtual void Render() = 0;

	const ComponentFieldDefinition& GetField() const;

	virtual ~FieldEditor() = default;

protected:
	ObjectComponent& m_Component;
	const ComponentFieldDefinition& m_Field;
	std::string m_FieldLabel;
};

class FieldEditorDefault
	: public FieldEditor
{
public:
	FieldEditorDefault(ObjectComponent& component, const ComponentFieldDefinition& field)
		: FieldEditor(component, field)
	{}

	virtual void Render() override;
};

class FieldEditorFloat
	: public FieldEditor
{
public:
	FieldEditorFloat(ObjectComponent& component, const ComponentFieldDefinition& field)
		: FieldEditor(component, field)
	{}

	virtual void Render() override;
};


class FieldEditorDouble
	: public FieldEditor
{
public:
	FieldEditorDouble(ObjectComponent& component, const ComponentFieldDefinition& field)
		: FieldEditor(component, field)
	{}

	virtual void Render() override;
};

class FieldEditorBool
	: public FieldEditor
{
public:
	FieldEditorBool(ObjectComponent& component, const ComponentFieldDefinition& field)
		: FieldEditor(component, field)
	{}

	virtual void Render() override;
};

class FieldEditorVector
	: public FieldEditor
{
public:
	FieldEditorVector(ObjectComponent& component, const ComponentFieldDefinition& field)
		: FieldEditor(component, field)
	{}

	virtual void Render() override;
};

class FieldEditorRgb8
	: public FieldEditor
{
public:
	FieldEditorRgb8(ObjectComponent& component, const ComponentFieldDefinition& field)
		: FieldEditor(component, field)
	{}

	virtual void Render() override;
};
