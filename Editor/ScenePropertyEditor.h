#pragma once
#include "IPropertyEditor.h"
#include "AssetHandles.h"

class ScenePropertyEditor 
    : public IPropertyEditor
{
public:
	virtual void RenderEmbedded() override;
	virtual bool HasValidObject() const override;

	void SetScene(std::weak_ptr<EditorUniversalHandle> scene);

private:
	EditorSceneHandle* GetSceneHandle();
	const EditorSceneHandle* GetSceneHandle() const;

private:
	std::weak_ptr<EditorUniversalHandle> m_SceneHandle;
};

