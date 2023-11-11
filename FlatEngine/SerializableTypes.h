#pragma once
#include <iostream>
#include <string>
#include "utility.h"
#include "Vector.h"

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

template<>
inline void ParseString<Vector>(const std::string& str, Vector& out_value)
{
	size_t comma_pos = str.find(',');
	FE_ASSERT(comma_pos != (size_t)-1, "Comma not found");

	std::string first_str = str.substr(0, comma_pos);

	FE_ASSERT(str.size() > comma_pos + 1, "Invalid vector string");
	std::string second_str = str.substr(comma_pos + 1);
	
	ParseString<float>(first_str, out_value.x);
	ParseString<float>(second_str, out_value.y);
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

template<>
inline std::string ToString<Vector>(const Vector& value)
{
	return std::to_string(value.x) + ", " + std::to_string(value.y);
}