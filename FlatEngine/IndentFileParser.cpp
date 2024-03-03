#include "IndentFileParser.h"

void SplitLineToKeyAndValue(const std::string& line, std::string& key, std::string& value)
{
    size_t colon_pos = line.find(':');

    key = line.substr(0, colon_pos);
    TrimWhitespaces(key);
    if (colon_pos == line.size() - 1 ||
        colon_pos == (size_t)-1)
    {
        value = "";
        return;
    }

    value = line.substr(colon_pos + 1);
    TrimWhitespaces(value);
}

void TrimWhitespaces(std::string& symbol)
{
    // Trim from left
    symbol.erase(
        symbol.begin(),
        std::find_if(symbol.begin(), symbol.end(), [](unsigned char c) {
            return !std::isspace(c);
            })
    );

    // Trim from right
    symbol.erase(
        std::find_if(symbol.rbegin(), symbol.rend(), [](unsigned char c) {
            return !std::isspace(c);
            }).base(),
                symbol.end()
                );
}
