#pragma once
#include "BTEnums.h"

namespace bt
{
	class Node
	{
	public:
		ENodeStatus Run();

	protected:
		virtual void Init() {}
		virtual ENodeStatus Update() = 0;
		virtual void Deinit() {}

	private:
		// TODO: probably not all nodes need to call Init or Deinit so we don't need to track status for them
		ENodeStatus m_CurrStatus = ENodeStatus::Invalid;
	};
}