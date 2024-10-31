#pragma once
#include "SceneSerializer.h"
#include "IndentFileParser.h"

enum class SceneParsingState
{
	SceneParams,
	GameObjectList
};

class SceneLoader
	: private IndentFileParser<SceneParsingState>
{
public:
	static std::unique_ptr<Scene> LoadScene(const char* file_path);
	static std::unique_ptr<SceneStringDesc> LoadSceneDesc(const char* file_path);

	static constexpr const char* s_GameObjectsKey = "game_objects";
	static constexpr const char* s_BackgroundColorKey = "background_color";

protected:
	virtual SceneParsingState GetOuterParsingState(SceneParsingState current_state) override;
	virtual void ParseLineForCurrentState(const std::string& line) override;

private:
	SceneLoader(std::fstream& file);

	std::unique_ptr<SceneStringDesc> LoadScene();

	void ParseSceneParamsLine(const std::string& line);
	void ParseGameObjectTypeLine(const std::string& line);

private:
	std::unique_ptr<SceneStringDesc> m_SceneDesc;
};

