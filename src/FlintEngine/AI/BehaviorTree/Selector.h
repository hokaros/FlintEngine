#pragma once
#include "Composite.h"

namespace bt
{
	class Selector
		: public Composite
	{
	protected:
		virtual void Init() override;
		virtual ENodeStatus Update() override;

	private:
		ChildrenT::iterator m_CurrentChild;
	};
}