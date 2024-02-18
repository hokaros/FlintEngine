#pragma once
#include <fstream>

#include "../FlatEngine/utility.h"

class IndentPrinter
{
public:
	IndentPrinter(std::fstream& file, size_t start_indent);

	void StartLine();
	void AddToLine(const std::string& str);
	void EndLine();

	void PrintLine(const std::string& str);

	void IncreaseIndent();
	void DecreaseIndent();

private:
	static std::string CreateIndent(size_t size);

private:
	std::fstream& m_File;
	size_t m_CurrIndent;
	std::string m_CurrIndentStr;
};

