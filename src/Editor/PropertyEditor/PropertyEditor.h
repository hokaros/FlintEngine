#pragma once
#include <Core/GameObject.h>
#include <IEditableGameObject.h>
#include "AssetHandles.h"
#include "SelectedObjectManager.h"
#include "GameObjectPropertyEditor.h"
#include "ScenePropertyEditor.h"

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

	void RenderSharedSection();

private:
	std::weak_ptr<EditorUniversalHandle> m_EditedObjectHandle;
	
	// Actual implementations of the Property Editor
	IPropertyEditor* m_CurrEditor = nullptr;
	GameObjectPropertyEditor m_GameObjectEditor;
	ScenePropertyEditor m_SceneEditor;
};

