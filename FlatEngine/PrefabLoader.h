#pragma once
#include "../FlatEngine/GameObjectSerializer.h"

class PrefabLoader
{
public:
	static std::unique_ptr<GameObject> LoadPrefab(const char* file_path);

private:
	enum class ParsingState
	{
		GameObjectParams,
		ComponentDefinitions,
		SpecificComponentDefinition,
		ChildDefinitions
	};

private:
	PrefabLoader(std::fstream& prefab_file, size_t start_indent);
	std::unique_ptr<GameObject> LoadPrefab();

	std::unique_ptr<GameObjectStringDesc> LoadGameObject(std::string& first_unconsumed_line);
	bool DispatchLine(const std::string& line);
	void ParseGameObjectParamLine(const std::string& line);

	void ParseComponentNameLine(const std::string& line);
	void ParseComponentFieldLine(const std::string& line);

	void FinalizeComponentLoading();

	void ParseChildTypeLine(const std::string& line);

	void SetParsingState(ParsingState state);
	void SetParsingStateAfterIndent(ParsingState state);
	void GoToOuterParsingState(size_t levels);
	void ParseLineForCurrentState(const std::string& line);

	static void SplitLineToKeyAndValue(const std::string& line, std::string& key, std::string& value);
	static void TrimWhitespaces(std::string& symbol);

private:
	ParsingState m_ParsingState = ParsingState::GameObjectParams;
	ParsingState m_ParsingStateAfterIndent = ParsingState::GameObjectParams;
	std::unique_ptr<GameObjectStringDesc> m_GameObjectDesc;
	ComponentStringDesc m_CurrComponentDesc;

	std::fstream& m_PrefabFile;

	std::string m_ReturnedLine;

	size_t m_PrevIndent;
	size_t m_CurrIndent;
	size_t m_StartIndent;
};

