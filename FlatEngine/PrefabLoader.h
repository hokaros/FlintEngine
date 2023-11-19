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
		SpecificComponentDefinition
	};

private:
	PrefabLoader() = default;
	std::unique_ptr<GameObject> LoadPrefab(std::fstream& file);
	void DispatchLine(const std::string& line);
	void ParseGameObjectParamLine(const std::string& line);

	void ParseComponentNameLine(const std::string& line);
	void ParseComponentFieldLine(const std::string& line);

	void FinalizeComponentLoading();

	void SetParsingState(ParsingState state);
	void SetParsingStateAfterIndent(ParsingState state);
	void GoToOuterParsingState();
	void ParseLineForCurrentState(const std::string& line);

	static void SplitLineToKeyAndValue(const std::string& line, std::string& key, std::string& value);
	static void TrimWhitespaces(std::string& symbol);

private:
	ParsingState m_ParsingState = ParsingState::GameObjectParams;
	ParsingState m_ParsingStateAfterIndent = ParsingState::GameObjectParams;
	GameObjectStringDesc m_GameObjectDesc;
	ComponentStringDesc m_CurrComponentDesc;
	size_t m_PrevIndent = 0;
};

