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
	void RenderSceneEditor(EditorSceneHandle& scene);
	void InitValuesFromScene(const Scene& scene);
	void ApplyValuesToScene(EditorSceneHandle& scene);

	void RenderNavmeshSection(Scene& scene);
	void RenderNavmeshGenerationParamsConfig();

	EditorSceneHandle* GetSceneHandle();
	const EditorSceneHandle* GetSceneHandle() const;

private:
	std::weak_ptr<EditorUniversalHandle> m_SceneHandle;

	Navigation::NavmeshGenerationParams m_NavmeshGenerationParams;
};

