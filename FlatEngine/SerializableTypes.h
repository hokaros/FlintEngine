#pragma once
#include <iostream>
#include <string>
#include <sstream>
#include "utility.h"
#include "Vector.h"
#include "Draw.h"

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
inline void ParseString<bool>(const std::string& str, bool& out_value)
{
	int int_value = std::stoi(str);
	FE_ASSERT(int_value == 0 || int_value == 1, "Invalid boolean string");
	
	if (int_value == 0)
	{
		out_value = false;
	}
	else
	{
		out_value = true;
	}
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

template<>
inline void ParseString<Rgb8>(const std::string& str, Rgb8& out_value)
{
	size_t first_comma_pos = str.find(',');
	FE_ASSERT(first_comma_pos != (size_t)-1, "Comma not found");

	std::string first_str = str.substr(0, first_comma_pos);

	std::string remainder = str.substr(first_comma_pos + 1);
	size_t comma_pos_in_remainder = remainder.find(',');
	FE_ASSERT(comma_pos_in_remainder != (size_t)-1, "Comma not found");

	std::string second_str = remainder.substr(0, comma_pos_in_remainder);
	std::string third_str = remainder.substr(comma_pos_in_remainder + 1);

	std::stringstream ss;
	int r, g, b;
	ss << std::hex << first_str;
	ss >> r;
	ss.clear();
	ss << std::hex << second_str;
	ss >> g;
	ss.clear();
	ss << std::hex << third_str;
	ss >> b;

	out_value.r = r;
	out_value.g = g;
	out_value.b = b;
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
inline std::string ToString<bool>(const bool& value)
{
	if (value)
	{
		return "1";
	}

	return "0";
}

template<>
inline std::string ToString<Vector>(const Vector& value)
{
	return std::to_string(value.x) + ", " + std::to_string(value.y);
}

template<>
inline std::string ToString<Rgb8>(const Rgb8& value)
{
	std::stringstream ss;
	ss << std::hex << (unsigned int)value.r;
	ss << ',' << std::hex << (unsigned int)value.g;
	ss << ',' << std::hex << (unsigned int)value.b;

	std::string result;
	ss >> result;
	return result;
}