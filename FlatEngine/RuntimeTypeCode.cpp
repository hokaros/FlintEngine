#include "RuntimeTypeCode.h" 

const RuntimeTypeCode RuntimeTypeCode::INVALID = RuntimeTypeCode(0);

RuntimeTypeCode::RuntimeTypeCode(unsigned int id)
	: m_Id(id)
{
}

RuntimeTypeCode RuntimeTypeCode::CreateNew()
{
	static unsigned int s_HighestId = 0;
	s_HighestId++;
	return RuntimeTypeCode(s_HighestId);
}

bool operator<(const RuntimeTypeCode& lhs, const RuntimeTypeCode& rhs)
{
	return lhs.m_Id < rhs.m_Id;
}
