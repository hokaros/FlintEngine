#pragma once
#include "Composite.h"
#include "Decorator.h"
#include <utility.h>

namespace bt
{
	template<typename CompositeT>
	class CompositeNodeBuilder
	{
	public:
		using NodeT = typename CompositeT::NodeT;

		CompositeNodeBuilder();

		CompositeNodeBuilder<CompositeT>& Child(std::unique_ptr<NodeT> child);
		std::unique_ptr<CompositeT> Finalize();

	private:
		std::unique_ptr<CompositeT> m_Result;
	};

	template<typename DecoratorT, typename... InitParamsT>
	class DecoratorNodeBuilder
	{
	public:
		using BuilderT = typename DecoratorNodeBuilder<DecoratorT, InitParamsT...>;
		using ChildT = typename DecoratorT::ChildT;

		DecoratorNodeBuilder(InitParamsT... params);

		BuilderT& Child(std::unique_ptr<ChildT> child);
		std::unique_ptr<DecoratorT> Finalize();

	private:
		std::unique_ptr<DecoratorT> m_Result;
	};

	class TreeBuilder
	{
	public:
		template<typename CompositeT>
		static std::unique_ptr<CompositeNodeBuilder<CompositeT>> Composite();

		template<typename DecoratorT, typename... InitParamsT>
		static std::unique_ptr<DecoratorNodeBuilder<DecoratorT, InitParamsT...>> Decorator(InitParamsT... params);
	};



	template<typename CompositeT>
	inline CompositeNodeBuilder<CompositeT>::CompositeNodeBuilder()
	{
		m_Result = std::make_unique<CompositeT>();
	}

	template<typename CompositeT>
	inline CompositeNodeBuilder<CompositeT>& CompositeNodeBuilder<CompositeT>::Child(std::unique_ptr<NodeT> child)
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


	template<typename DecoratorT, typename... InitParamsT>
	inline DecoratorNodeBuilder<DecoratorT, InitParamsT...>::DecoratorNodeBuilder(InitParamsT... params)
	{
		m_Result = std::make_unique<DecoratorT>(params...);
	}

	template<typename DecoratorT, typename... InitParamsT>
	inline auto DecoratorNodeBuilder<DecoratorT, InitParamsT...>::Child(std::unique_ptr<ChildT> child) -> BuilderT&
	{
		FE_ASSERT(m_Result != nullptr, "Already finalized");

		m_Result->SetChild(std::move(child));

		return *this;
	}

	template<typename DecoratorT, typename... InitParamsT>
	inline std::unique_ptr<DecoratorT> DecoratorNodeBuilder<DecoratorT, InitParamsT...>::Finalize()
	{
		return std::move(m_Result);
	}


	template<typename CompositeT>
	inline std::unique_ptr<CompositeNodeBuilder<CompositeT>> TreeBuilder::Composite()
	{
		return std::make_unique<CompositeNodeBuilder<CompositeT>>();
	}

	template<typename DecoratorT, typename... InitParamsT>
	inline std::unique_ptr<DecoratorNodeBuilder<DecoratorT, InitParamsT...>> TreeBuilder::Decorator(InitParamsT... params)
	{
		return std::make_unique<DecoratorNodeBuilder<DecoratorT, InitParamsT...>>(params...);
	}
}