#pragma once
#include <iostream>
#include <string>
#include "utility.h"

template<typename T>
void ParseString(const std::string& str, T& out_value)
{
	FE_ASSERT(false, "Parsing not supported for this type");
}

template<>
inline void ParseString<float>(const std::string& str, float& out_value)
{
	out_value = std::stof(str);
}

template<>
inline void ParseString<int>(const std::string& str, int& out_value)
{
	out_value = std::stoi(str);
}


template<typename T>
std::string ToString(const T& value)
{
	FE_ASSERT(false, "ToString() not supported for this type");
}

template<>
inline std::string ToString<float>(const float& value)
{
	return std::to_string(value);
}

template<>
inline std::string ToString<int>(const int& value)
{
	return std::to_string(value);
}