#include "IndentPrinter.h"


IndentPrinter::IndentPrinter(std::fstream& file, size_t start_indent)
    : m_File(file)
    , m_CurrIndent(start_indent)
    , m_CurrIndentStr("")
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

std::string IndentPrinter::CreateIndent(size_t size)
{
    std::string indent_str = "";

    for (size_t i = 0; i < size; i++)
    {
        indent_str += "\t";
    }

    return indent_str;
}
