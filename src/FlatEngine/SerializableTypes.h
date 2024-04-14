#pragma once
#include <iostream>
#include <string>
#include <sstream>
#include "utility.h"
#include "Vector.h"
#include "Draw.h"
#include "RuntimeTypeCode.h"

template<typename T>
class SerializableTypeInterface
{
public:
	static void ParseString(const std::string& str, T& out_value)
	{
		FE_ASSERT(false, "Parsing not supported for this type");
	}

	static std::string ToString(const T& value)
	{
		FE_ASSERT(false, "ToString() not supported for this type");
	}

	static RuntimeTypeCode GetTypeCode()
	{
		FE_ASSERT(false, "Trying to access typecode of a non-existing type");
		return RuntimeTypeCode::INVALID;
	}
};

template<typename T>
using STI = SerializableTypeInterface<T>;

#define STI_DEFINE_TYPECODE_GETTER()									\
static inline RuntimeTypeCode GetTypeCode()								\
{																		\
	static RuntimeTypeCode s_TypeCode = RuntimeTypeCode::CreateNew();	\
	return s_TypeCode;													\
}


template<>
class SerializableTypeInterface<std::string>
{
public:
	static inline void ParseString(const std::string& str, std::string& out_value)
	{
		out_value = str;
	}
	static inline std::string ToString(const std::string& value)
	{
		return value;
	}

	STI_DEFINE_TYPECODE_GETTER()
};

template<>
class SerializableTypeInterface<float>
{
public:
	static inline void ParseString(const std::string& str, float& out_value)
	{
		out_value = std::stof(str);
	}
	static inline std::string ToString(const float& value)
	{
		return std::to_string(value);
	}

	STI_DEFINE_TYPECODE_GETTER()
};

template<>
class SerializableTypeInterface<double>
{
public:
	static inline void ParseString(const std::string& str, double& out_value)
	{
		out_value = std::stod(str);
	}
	static inline std::string ToString(const double& value)
	{
		return std::to_string(value);
	}

	STI_DEFINE_TYPECODE_GETTER()
};

template<>
class SerializableTypeInterface<int>
{
public:
	static inline void ParseString(const std::string& str, int& out_value)
	{
		out_value = std::stoi(str);
	}
	static inline std::string ToString(const int& value)
	{
		return std::to_string(value);
	}
	
	STI_DEFINE_TYPECODE_GETTER()
};

template<>
class SerializableTypeInterface<bool>
{
public:
	static inline void ParseString(const std::string& str, bool& out_value)
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
	static inline std::string ToString(const bool& value)
	{
		if (value)
		{
			return "1";
		}

		return "0";
	}

	STI_DEFINE_TYPECODE_GETTER()
};

template<>
class SerializableTypeInterface<Vector>
{
public:
	static inline void ParseString(const std::string& str, Vector& out_value)
	{
		size_t comma_pos = str.find(',');
		FE_ASSERT(comma_pos != (size_t)-1, "Comma not found");

		std::string first_str = str.substr(0, comma_pos);

		FE_ASSERT(str.size() > comma_pos + 1, "Invalid vector string");
		std::string second_str = str.substr(comma_pos + 1);

		SerializableTypeInterface<float>::ParseString(first_str, out_value.x);
		SerializableTypeInterface<float>::ParseString(second_str, out_value.y);
	}
	static inline std::string ToString(const Vector& value)
	{
		return std::to_string(value.x) + ", " + std::to_string(value.y);
	}

	STI_DEFINE_TYPECODE_GETTER()
};

template<>
class SerializableTypeInterface<Rgb8>
{
public:
	static inline void ParseString(const std::string& str, Rgb8& out_value)
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
	static inline std::string ToString(const Rgb8& value)
	{
		std::stringstream ss;
		ss << std::hex << (unsigned int)value.r;
		ss << ',' << std::hex << (unsigned int)value.g;
		ss << ',' << std::hex << (unsigned int)value.b;

		std::string result;
		ss >> result;
		return result;
	}

	STI_DEFINE_TYPECODE_GETTER()
};

