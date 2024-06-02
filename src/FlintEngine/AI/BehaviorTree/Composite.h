#pragma once
#include <vector>
#include <memory>

#include "Node.h"

namespace bt
{
	// Behavior Tree node with multiple children
	template<typename ContextT>
	class Composite
		: public Node<ContextT>
	{
	public:
		using NodeT = Node<ContextT>;
		using ChildrenT = std::vector<std::unique_ptr<NodeT>>;

		void AddChild(std::unique_ptr<NodeT> node);
		const ChildrenT& GetChildren() const;
		
	protected:
		ChildrenT& GetChildren();

	private:
		ChildrenT m_Children;
	};



	template<typename ContextT>
	void Composite<ContextT>::AddChild(std::unique_ptr<NodeT> node)
	{
		m_Children.push_back(std::move(node));
	}

	template<typename ContextT>
	const typename Composite<ContextT>::ChildrenT& Composite<ContextT>::GetChildren() const
	{
		return m_Children;
	}

	template<typename ContextT>
	typename Composite<ContextT>::ChildrenT& Composite<ContextT>::GetChildren()
	{
		return m_Children;
	}
}