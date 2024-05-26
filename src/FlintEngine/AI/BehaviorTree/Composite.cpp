#include "Composite.h"

namespace bt
{
	void Composite::AddChild(std::unique_ptr<Node> node)
	{
		m_Children.push_back(std::move(node));
	}

	const Composite::ChildrenT& Composite::GetChildren() const
	{
		return m_Children;
	}

	Composite::ChildrenT& Composite::GetChildren()
	{
		return m_Children;
	}
}