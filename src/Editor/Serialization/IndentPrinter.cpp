#include "IndentPrinter.h"


IndentPrinter::IndentPrinter(std::fstream& file, size_t start_indent)
    : m_File(file)
    , m_CurrIndent(start_indent)
    , m_CurrIndentStr("")
{
}

IndentPrinter::IndentPrinter(const IndentPrinter& other)
    : m_File(other.m_File)
    , m_CurrIndent(other.m_CurrIndent)
    , m_CurrIndentStr(other.m_CurrIndentStr)
{
}

void IndentPrinter::StartLine()
{
    m_File << m_CurrIndentStr << "- ";
}

void IndentPrinter::AddToLine(const std::string& str)
{
    m_File << str;
}

void IndentPrinter::EndLine()
{
    m_File << std::endl;
}

void IndentPrinter::PrintLine(const std::string& str)
{
    StartLine();
    AddToLine(str);
    EndLine();
}

void IndentPrinter::IncreaseIndent()
{
    m_CurrIndent++;
    m_CurrIndentStr = CreateIndent(m_CurrIndent);
}

void IndentPrinter::DecreaseIndent()
{
    if (m_CurrIndent == 0)
    {
        FE_WARN("Indent is already 0");
        return;
    }

    m_CurrIndent--;
    m_CurrIndentStr = CreateIndent(m_CurrIndent);
}

void IndentPrinter::SaveKeyValuePair(const std::string& key, const std::string& value)
{
    StartLine();
    AddToLine(key);
    AddToLine(": ");
    AddToLine(value);
    EndLine();
}

void IndentPrinter::SaveKeyValuePairs(const std::map<std::string, std::string>& dict)
{
    for (auto& pair : dict)
    {
        const std::string& key = pair.first;
        const std::string& value = pair.second;

        SaveKeyValuePair(key, value);
    }
}

std::string IndentPrinter::CreateIndent(size_t size)
{
    std::string indent_str = "";

    for (size_t i = 0; i < size; i++)
    {
        indent_str += "\t";
    }

    return indent_str;
}
