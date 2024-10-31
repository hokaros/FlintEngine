#pragma once
#include <Serialization/SceneSerializer.h>
#include "IndentPrinter.h"

class SceneSaver
{
public:
	static void SaveScene(const Scene& scene, const char* file_path);

private:
	SceneSaver(std::fstream& scene_file);

	void SaveScene(const Scene& scene);

	void SaveSceneParameters(const SceneStringDesc& scene);
	void SaveSceneGameObjects(const SceneStringDesc& scene);

private:
	IndentPrinter m_IndentPrinter;
};

