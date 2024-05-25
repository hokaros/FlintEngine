#pragma once
#include "Composite.h"

namespace bt
{
	class Selector
		: public Composite
	{
	public:
		virtual ENodeResult Run() override;
	};
}