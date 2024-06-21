#pragma once
#include "GameObjectSerializer.h"
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
	std::unique_ptr<GameObjectStringDesc> LoadPrefabDesc();

	std::unique_ptr<GameObjectStringDesc> LoadGameObject(std::string& first_unconsumed_line);

	static std::unique_ptr<GameObject> LoadPrefab(const char* file_path);
	static std::unique_ptr<GameObjectStringDesc> LoadPrefabDesc(const char* file_path);

	static std::unique_ptr<GameObjectStringDescProxy> ParseChildByKey(const std::string& key, std::fstream& file, size_t start_indent, std::string& first_unconsumed_line);

protected:
	virtual GameObjectParsingState GetOuterParsingState(GameObjectParsingState current_state) override;
	virtual void ParseLineForCurrentState(const std::string& line) override;
	virtual void SetParsingState(GameObjectParsingState state) override;

private:
	void ParseGameObjectParamLine(const std::string& line);

	void ParseComponentNameLine(const std::string& line);
	void ParseComponentFieldLine(const std::string& line);

	void FinalizeComponentLoading();

	void ParseChildTypeLine(const std::string& line);

private:
	std::unique_ptr<GameObjectStringDesc> m_GameObjectDesc;
	ComponentStringDesc m_CurrComponentDesc;
};

