#pragma once
#include "Composite.h"

namespace bt
{
	class Sequence
		: public Composite
	{
	public:
		virtual ENodeResult Run() override;
	};
}