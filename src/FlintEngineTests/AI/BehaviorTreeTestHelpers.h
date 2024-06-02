#pragma once
#include <AI/BehaviorTree/Composite.h>

class BehaviorTreeTestHelpers
{
public:
	template<typename T, typename ContextT>
	static T& AddChildTo(bt::Composite<ContextT>& parent)
	{
		std::unique_ptr<T> child = std::make_unique<T>();
		T& child_ref = *child;
		parent.AddChild(std::move(child));

		return child_ref;
	}
};