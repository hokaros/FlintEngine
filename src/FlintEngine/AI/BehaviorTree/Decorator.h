#pragma once
#include "Node.h"
#include <memory>

namespace bt
{
	template<typename ContextT>
	class Decorator
		: public Node<ContextT>
	{
	public:
		using ChildT = Node<ContextT>;

		void SetChild(std::unique_ptr<ChildT> child);
		ChildT* GetChild();

	private:
		std::unique_ptr<ChildT> m_Child = nullptr;
	};



	template<typename ContextT>
	inline auto Decorator<ContextT>::SetChild(std::unique_ptr<ChildT> child) -> void
	{
		m_Child = std::move(child);
	}

	template<typename ContextT>
	inline auto Decorator<ContextT>::GetChild() -> ChildT*
	{
		return m_Child.get();
	}
}