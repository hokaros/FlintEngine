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
		using ChildrenT = std::vector<std::unique_ptr<Node>>;

		void AddChild(std::unique_ptr<Node> node);
		const ChildrenT& GetChildren() const;
		
	protected:
		ChildrenT& GetChildren();

	private:
		ChildrenT m_Children;
	};
}