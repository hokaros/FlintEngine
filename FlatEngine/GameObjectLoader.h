#pragma once
#include "../FlatEngine/GameObjectSerializer.h"
#include "IndentFileParser.h"

enum class GameObjectParsingState
{
	GameObjectParams,
	ComponentDefinitions,
	SpecificComponentDefinition,
	ChildDefinitions
};

class GameObjectLoader
	: private IndentFileParser<GameObjectParsingState>
{
public:
	GameObjectLoader(std::fstream& file, size_t start_indent);
	std::unique_ptr<GameObject> LoadPrefab();

	std::unique_ptr<GameObjectStringDesc> LoadGameObject(std::string& first_unconsumed_line);

	static std::unique_ptr<GameObject> LoadPrefab(const char* file_path);

protected:
	virtual void GoToOuterParsingState(size_t levels) override;
	virtual void ParseLineForCurrentState(const std::string& line) override;

private:
	void ParseGameObjectParamLine(const std::string& line);

	void ParseComponentNameLine(const std::string& line);
	void ParseComponentFieldLine(const std::string& line);

	void FinalizeComponentLoading();

	void ParseChildTypeLine(const std::string& line);

	void SetParsingState(GameObjectParsingState state);

private:
	std::unique_ptr<GameObjectStringDesc> m_GameObjectDesc;
	ComponentStringDesc m_CurrComponentDesc;
};

