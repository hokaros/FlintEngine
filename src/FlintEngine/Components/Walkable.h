#pragma once
#include <ComponentDefinition.h>

class Walkable
	: public ObjectComponent
{
	DECLARE_COMPONENT();
public:
	const Vector& GetSize() const;

private:
	Vector m_Size;
	DECLARE_FIELD(m_Size);
};