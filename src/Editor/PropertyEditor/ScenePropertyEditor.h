#pragma once
#include "IPropertyEditor.h"
#include <AssetHandles/EditorUniversalHandle.h>

class ScenePropertyEditor 
    : public IPropertyEditor
{
public:
	virtual void RenderEmbedded() override;
	virtual bool HasValidObject() const override;

	void SetScene(std::weak_ptr<EditorUniversalHandle> scene);

private:
	void RenderSceneEditor(EditableScene& scene);
	void InitValuesFromScene(EditableScene& scene);
	void ApplyValuesToScene(EditableScene& scene);

	EditorSceneHandle* GetSceneHandle();
	const EditorSceneHandle* GetSceneHandle() const;

private:
	std::weak_ptr<EditorUniversalHandle> m_SceneHandle;
};

