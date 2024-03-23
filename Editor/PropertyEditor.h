#pragma once
#include "../FlatEngine/GameObject.h"
#include "../FlatEngine/IEditableGameObject.h"
#include "GameObjectPropertyEditor.h"
#include "AssetHandles.h"
#include "SelectedObjectManager.h"

class PropertyEditor
	: protected ISelectionObserver
{
public:
	void Init(SelectedObjectManager& selected_game_object_manager);

	void Render();

protected:
	// ISelectionObserver
	virtual void OnObjectSelected(std::weak_ptr<EditorUniversalHandle> object) override;

private:
	void ApplyObjectToEditors(std::weak_ptr<EditorUniversalHandle> object);
	IPropertyEditor* GetValidEditor();
	void ValidateCurrentEditor();

private:
	std::weak_ptr<EditorUniversalHandle> m_GameObjectHandle; // TODO: rename
	
	// Actual implementations of the Property Editor
	IPropertyEditor* m_CurrEditor;
	GameObjectPropertyEditor m_GameObjectEditor;
};

