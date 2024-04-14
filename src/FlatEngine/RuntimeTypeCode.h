#pragma once

class RuntimeTypeCode
{
public:
	RuntimeTypeCode(unsigned int id);

	static RuntimeTypeCode CreateNew();

	bool operator ==(const RuntimeTypeCode& other) const;
	friend bool operator <(const RuntimeTypeCode& lhs, const RuntimeTypeCode& rhs);

public:
	static const RuntimeTypeCode INVALID;

private:
	unsigned int m_Id;
};


#define DECLARE_RTC()												\
public:																\
	virtual RuntimeTypeCode GetTypeCode() const override;


#define DEFINE_RTC(clazz)											\
RuntimeTypeCode clazz::GetTypeCode() const							\
{																	\
static RuntimeTypeCode s_TypeCode = RuntimeTypeCode::CreateNew();	\
return s_TypeCode;													\
}	
