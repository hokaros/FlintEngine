#pragma once
#include "BTEnums.h"

namespace bt
{
	class Node
	{
	public:
		virtual ENodeResult Run() = 0;
	};
}