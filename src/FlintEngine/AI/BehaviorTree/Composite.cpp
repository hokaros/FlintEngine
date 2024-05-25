#include "Composite.h"

namespace bt
{
	void Composite::AddChild(std::unique_ptr<Node> node)
	{
		m_Children.push_back(std::move(node));
	}

	const std::vector<std::unique_ptr<Node>>& Composite::GetChildren() const
	{
		return m_Children;
	}
}