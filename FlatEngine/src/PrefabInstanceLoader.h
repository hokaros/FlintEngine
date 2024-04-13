#pragma once
#include "PrefabInstanceSerializer.h"
#include "IndentFileParser.h"

enum class PrefabInstanceParsingState
{
	GeneralParams,
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

private:
	void ParseLineGeneralParams(const std::string& line);

private:
	std::unique_ptr<PrefabInstanceStringDesc> m_PrefabInstanceDesc;
};

