#pragma once
#include <fstream>
#include <map>

#include "../FlatEngine/utility.h"

class IndentPrinter
{
public:
	IndentPrinter(std::fstream& file, size_t start_indent);
	IndentPrinter(const IndentPrinter& other);

	void StartLine();
	void AddToLine(const std::string& str);
	void EndLine();

	void PrintLine(const std::string& str);

	void IncreaseIndent();
	void DecreaseIndent();

	void SaveKeyValuePair(const std::string& key, const std::string& value);
	void SaveKeyValuePairs(const std::map<std::string, std::string>& dict);

private:
	static std::string CreateIndent(size_t size);

private:
	std::fstream& m_File;
	size_t m_CurrIndent;
	std::string m_CurrIndentStr;
};

