#pragma once
#include <fstream>

#include "utility.h"

void SplitLineToKeyAndValue(const std::string& line, std::string& key, std::string& value);
void TrimWhitespaces(std::string& symbol);

template<typename ParsingStateT>
class IndentFileParser
{
protected:
    IndentFileParser(std::fstream& file, size_t start_indent, ParsingStateT start_state);

	void Load(std::string& first_unconsumed_line);

    virtual void SetParsingState(ParsingStateT state);
    void SetParsingStateAfterIndent(ParsingStateT state);

    virtual ParsingStateT GetOuterParsingState(ParsingStateT current_state) = 0;
	virtual void ParseLineForCurrentState(const std::string& line) = 0;

protected:
	ParsingStateT m_ParsingState;
	ParsingStateT m_ParsingStateAfterIndent;

    std::fstream& m_File;

    std::string m_ReturnedLine;
    size_t m_PrevIndent;
    size_t m_CurrIndent;

private:
	bool DispatchLine(const std::string& line);
    void GoToOuterParsingState(size_t levels);

private:
    size_t m_StartIndent;
};



template<typename ParsingStateT>
inline IndentFileParser<ParsingStateT>::IndentFileParser(std::fstream& file, size_t start_indent, ParsingStateT start_state)
    : m_File(file)
    , m_StartIndent(start_indent)
    , m_PrevIndent(start_indent)
    , m_CurrIndent(start_indent)
    , m_ReturnedLine("")
    , m_ParsingState(start_state)
    , m_ParsingStateAfterIndent(start_state)
{
}

template<typename ParsingStateT>
inline void IndentFileParser<ParsingStateT>::Load(std::string& first_unconsumed_line)
{
    first_unconsumed_line = "";

    char line[256];
    while (!m_File.eof())
    {
        std::string line_str;
        if (m_ReturnedLine.size() > 0)
        {
            line_str = m_ReturnedLine;
            m_ReturnedLine = "";
        }
        else
        {
            m_File.getline(line, 256);
            line_str = line;
        }

        bool line_consumed = DispatchLine(line_str);
        if (!line_consumed)
        {
            first_unconsumed_line = line_str;
            break;
        }
    }

    // Go to the most outer state
    GoToOuterParsingState(m_PrevIndent);
}

template<typename ParsingStateT>
inline void IndentFileParser<ParsingStateT>::SetParsingStateAfterIndent(ParsingStateT state)
{
    m_ParsingStateAfterIndent = state;
}

template<typename ParsingStateT>
inline void IndentFileParser<ParsingStateT>::SetParsingState(ParsingStateT state)
{
    m_ParsingState = state;
    m_ParsingStateAfterIndent = state;
}

template<typename ParsingStateT>
inline bool IndentFileParser<ParsingStateT>::DispatchLine(const std::string& line)
{
    m_CurrIndent = line.find('-');
    if (m_CurrIndent >= line.size() - 1)
    {
        if (line.size() > 0)
        {
            FE_DATA_ERROR("No '-' at the start of line");
        }
        return true;
    }

    if (m_CurrIndent < m_StartIndent)
    {
        // Outer scope
        return false;
    }

    std::string line_unindented = line.substr(m_CurrIndent + 1);

    if (m_CurrIndent < m_PrevIndent)
    {
        GoToOuterParsingState(m_PrevIndent - m_CurrIndent);
    }
    else if (m_CurrIndent > m_PrevIndent)
    {
        SetParsingState(m_ParsingStateAfterIndent);
    }
    else
    {
        SetParsingState(m_ParsingState);
    }

    ParseLineForCurrentState(line_unindented);

    m_PrevIndent = m_CurrIndent;

    return true;
}

template<typename ParsingStateT>
inline void IndentFileParser<ParsingStateT>::GoToOuterParsingState(size_t levels)
{
    for (size_t i = 0; i < levels; i++)
    {
        SetParsingState(GetOuterParsingState(m_ParsingState));
    }
}
