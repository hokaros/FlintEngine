#include "Walkable.h"

DEFINE_COMPONENT(Walkable);
DEFINE_FIELD(Walkable, m_Size);

const Vector& Walkable::GetSize() const
{
	return m_Size;
}