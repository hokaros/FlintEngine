#pragma once
#include "../FlatEngine/PrefabInstanceSerializer.h"
#include "IndentFileParser.h"

enum class PrefabInstanceParsingState
{
	GameObjectParams
	// TODO
};

class PrefabInstanceLoader
	: private IndentFileParser<PrefabInstanceParsingState>
{
public:
	PrefabInstanceLoader(std::fstream& file, size_t start_indent);
	std::unique_ptr<PrefabInstanceStringDesc> LoadPrefabInstanceDesc();

	std::unique_ptr<PrefabInstanceStringDesc> LoadPrefabInstanceDesc(std::string& first_unconsumed_line);

protected:
	virtual PrefabInstanceParsingState GetOuterParsingState(PrefabInstanceParsingState current_state) override;
	virtual void ParseLineForCurrentState(const std::string& line) override;
};

