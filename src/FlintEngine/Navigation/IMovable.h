#pragma once
#include <Math/Vector.h>

class IMovable
{
public:
	virtual void MoveTowards(const Vector& pos) = 0;
	virtual void Stop() = 0;
	virtual Vector GetPosition() const = 0;

	virtual ~IMovable() = default;
};