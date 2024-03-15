#pragma once
#include "../FlatEngine/ComponentFieldDefinition.h"
#include "../FlatEngine/imgui/imgui.h"
#include "../FlatEngine/IEditableGameObject.h"

class FieldEditor
{
public:
	FieldEditor(IEditableGameObject& game_object, ObjectComponent& component, const ComponentFieldDefinition& field);

	virtual void Render() = 0;

	const ComponentFieldDefinition& GetField() const;

	virtual ~FieldEditor() = default;

protected:
	template<typename ValueT>
	void ApplyToGameObject(const ValueT& value);

protected:
	IEditableGameObject& m_GameObject;
	ObjectComponent& m_Component;
	const ComponentFieldDefinition& m_Field;
	std::string m_FieldLabel;
};

#define FIELD_EDITOR_DELEGATING_CONSTRUCTOR(derived_clazz)																\
derived_clazz(IEditableGameObject& game_object, ObjectComponent& component, const ComponentFieldDefinition& field)		\
	: FieldEditor(game_object, component, field)																		\
{}

class FieldEditorDefault
	: public FieldEditor
{
public:
	FIELD_EDITOR_DELEGATING_CONSTRUCTOR(FieldEditorDefault);

	virtual void Render() override;
};

class FieldEditorInt
	: public FieldEditor
{
public:
	FIELD_EDITOR_DELEGATING_CONSTRUCTOR(FieldEditorInt);

	virtual void Render() override;
};

class FieldEditorFloat
	: public FieldEditor
{
public:
	FIELD_EDITOR_DELEGATING_CONSTRUCTOR(FieldEditorFloat);

	virtual void Render() override;
};


class FieldEditorDouble
	: public FieldEditor
{
public:
	FIELD_EDITOR_DELEGATING_CONSTRUCTOR(FieldEditorDouble);

	virtual void Render() override;
};

class FieldEditorBool
	: public FieldEditor
{
public:
	FIELD_EDITOR_DELEGATING_CONSTRUCTOR(FieldEditorBool);

	virtual void Render() override;
};

class FieldEditorVector
	: public FieldEditor
{
public:
	FIELD_EDITOR_DELEGATING_CONSTRUCTOR(FieldEditorVector);

	virtual void Render() override;
};

class FieldEditorRgb8
	: public FieldEditor
{
public:
	FIELD_EDITOR_DELEGATING_CONSTRUCTOR(FieldEditorRgb8);

	virtual void Render() override;
};


template<typename ValueT>
inline void FieldEditor::ApplyToGameObject(const ValueT& value)
{
	std::unique_ptr<ComponentFieldChangeContained<ValueT>> change = std::make_unique<ComponentFieldChangeContained<ValueT>>();
	change->component = &m_Component;
	change->field = &m_Field;
	change->SetValue(value);

	m_GameObject.ModifyComponentField(std::move(change));
}



#undef FIELD_EDITOR_DELEGATING_CONSTRUCTOR