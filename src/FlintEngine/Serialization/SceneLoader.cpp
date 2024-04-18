#include "SceneLoader.h"

#include <fstream>
#include "GameObjectLoader.h"

std::unique_ptr<EditableScene> SceneLoader::LoadScene(const char* file_path)
{
	std::unique_ptr<SceneStringDesc> scene_serialized = LoadSceneDesc(file_path);
	if (scene_serialized == nullptr)
		return nullptr;

	return SceneSerializer::Deserialize(*scene_serialized);
}

std::unique_ptr<SceneStringDesc> SceneLoader::LoadSceneDesc(const char* file_path)
{
	std::fstream scene_file;
	scene_file.open(file_path, std::ios::in);
	if (!scene_file.is_open())
	{
		FE_LOG("Couldn't open scene file: %s", file_path);
		return nullptr;
	}

	SceneLoader scene_loader(scene_file);
	std::unique_ptr<SceneStringDesc> scene = scene_loader.LoadScene();

	scene_file.close();
	return scene;
}

SceneParsingState SceneLoader::GetOuterParsingState(SceneParsingState current_state)
{
	switch (current_state)
	{
	case SceneParsingState::GameObjectList:
		return SceneParsingState::SceneParams;

	default:
		return SceneParsingState::SceneParams;
	}
}

void SceneLoader::ParseLineForCurrentState(const std::string& line)
{
	switch (m_ParsingState)
	{
	case SceneParsingState::SceneParams:
		ParseSceneParamsLine(line);
		break;
		
	case SceneParsingState::GameObjectList:
		ParseGameObjectTypeLine(line);
		break;
	}
}

SceneLoader::SceneLoader(std::fstream& file)
	: IndentFileParser<SceneParsingState>(file, 0, SceneParsingState::SceneParams)
{

}

std::unique_ptr<SceneStringDesc> SceneLoader::LoadScene()
{
	m_SceneDesc = std::make_unique<SceneStringDesc>();

	std::string first_unconsumed_line;
	Load(first_unconsumed_line);

	FE_ASSERT(m_ParsingState == SceneParsingState::SceneParams, "Should have reached most outer parsing state");

	return std::move(m_SceneDesc);
}

void SceneLoader::ParseSceneParamsLine(const std::string& line)
{
	std::string key, value;
	SplitLineToKeyAndValue(line, key, value);

	if (key == s_GameObjectsKey)
	{
		FE_DATA_CHECK(value.size() == 0, "Invalid value - GameObjects should be below");

		SetParsingStateAfterIndent(SceneParsingState::GameObjectList);
	}
	else if(key == s_BackgroundColorKey)
	{
		m_SceneDesc->background_color = value;
	}
}

void SceneLoader::ParseGameObjectTypeLine(const std::string& line)
{
	std::string key, value;
	SplitLineToKeyAndValue(line, key, value);

	std::unique_ptr<GameObjectStringDescProxy> game_object_desc = GameObjectLoader::ParseChildByKey(key.c_str(), m_File, m_CurrIndent, m_ReturnedLine);
	if (game_object_desc != nullptr)
	{
		m_SceneDesc->game_objects.push_back(std::move(game_object_desc));
	}
}
