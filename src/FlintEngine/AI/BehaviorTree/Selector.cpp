#include "Selector.h"

namespace bt
{
	ENodeResult Selector::Run()
	{
		for (const std::unique_ptr<Node>& child : GetChildren())
		{
			ENodeResult result = child->Run();

			if (result != ENodeResult::Failure)
				return result; // Success or InProgress should return without proceeding further
		}

		return ENodeResult::Failure; // Everything was a failure
	}
}