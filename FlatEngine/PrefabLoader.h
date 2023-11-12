#pragma once
#include "../FlatEngine/GameObjectSerializer.h"

class PrefabLoader
{
public:
	static GameObject* LoadPrefab(const char* file_path);

private:
	enum class ParsingState
	{
		GameObjectParams,
		ComponentDefinitions,
		SpecificComponentDefinition
	};

private:
	PrefabLoader() = default;
	GameObject* LoadPrefab(std::fstream& file);
	void ParseGameObjectParamLine(const std::string& line);

	void ParseComponentNameLine(const std::string& line);
	void ParseComponentFieldLine(const std::string& line);

	void FinalizeComponentLoading();

	void SetParsingState(ParsingState state);

	static void SplitLineToKeyAndValue(const std::string& line, std::string& key, std::string& value);
	static void TrimWhitespaces(std::string& symbol);

private:
	ParsingState m_ParsingState = ParsingState::GameObjectParams;
	GameObjectStringDesc m_GameObjectDesc;
	ComponentStringDesc m_CurrComponentDesc;
};

