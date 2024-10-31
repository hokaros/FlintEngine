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
	void RenderSceneEditor(Scene& scene);
	void InitValuesFromScene(Scene& scene);
	void ApplyValuesToScene(Scene& scene);

	void RenderActions(Scene& scene);

	EditorSceneHandle* GetSceneHandle();
	const EditorSceneHandle* GetSceneHandle() const;

private:
	std::weak_ptr<EditorUniversalHandle> m_SceneHandle;
};

