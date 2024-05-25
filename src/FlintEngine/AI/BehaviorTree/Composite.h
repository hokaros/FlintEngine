#pragma once
#include <vector>
#include <memory>

#include "Node.h"

namespace bt
{
	// Behavior Tree node with multiple children
	class Composite
		: public Node
	{
	public:
		void AddChild(std::unique_ptr<Node> node);
		const std::vector<std::unique_ptr<Node>>& GetChildren() const;

	private:
		std::vector<std::unique_ptr<Node>> m_Children;
	};
}