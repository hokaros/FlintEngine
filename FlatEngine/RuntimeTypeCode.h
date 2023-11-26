#pragma once

class RuntimeTypeCode
{
public:
	RuntimeTypeCode(unsigned int id);

	static RuntimeTypeCode CreateNew();

	friend bool operator <(const RuntimeTypeCode& lhs, const RuntimeTypeCode& rhs);

public:
	static const RuntimeTypeCode INVALID;

private:
	unsigned int m_Id;
};
