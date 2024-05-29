#pragma once
#include "Composite.h"
#include <utility.h>

namespace bt
{
	template<typename CompositeT>
	class CompositeNodeBuilder
	{
		static_assert(std::is_base_of<Composite, CompositeT>());

	public:
		CompositeNodeBuilder();

		CompositeNodeBuilder<CompositeT>& Child(std::unique_ptr<Node> child);
		std::unique_ptr<CompositeT> Finalize();

	private:
		std::unique_ptr<CompositeT> m_Result;
	};

	class TreeBuilder
	{
	public:
		template<typename CompositeT>
		static std::unique_ptr<CompositeNodeBuilder<CompositeT>> Composite();
	};



	template<typename CompositeT>
	inline CompositeNodeBuilder<CompositeT>::CompositeNodeBuilder()
	{
		m_Result = std::make_unique<CompositeT>();
	}

	template<typename CompositeT>
	inline CompositeNodeBuilder<CompositeT>& CompositeNodeBuilder<CompositeT>::Child(std::unique_ptr<Node> child)
	{
		FE_ASSERT(m_Result != nullptr, "Already finalized");

		m_Result->AddChild(std::move(child));

		return *this;
	}

	template<typename CompositeT>
	inline std::unique_ptr<CompositeT> CompositeNodeBuilder<CompositeT>::Finalize()
	{
		return std::move(m_Result);
	}

	template<typename CompositeT>
	inline std::unique_ptr<CompositeNodeBuilder<CompositeT>> TreeBuilder::Composite()
	{
		return std::make_unique<CompositeNodeBuilder<CompositeT>>();
	}
}