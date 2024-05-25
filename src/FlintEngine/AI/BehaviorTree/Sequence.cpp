#include "Sequence.h"

namespace bt
{
	ENodeResult Sequence::Run()
	{
		for (const std::unique_ptr<Node>& child : GetChildren())
		{
			ENodeResult result = child->Run();

			if (result != ENodeResult::Success)
				return result;
		}

		return ENodeResult::Success; // Everything was a success (great success)
	}
}