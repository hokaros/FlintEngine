#pragma once

namespace dt
{
	class ICondition
	{
	public:
		virtual bool Test() const = 0;

		virtual ~ICondition() = default;
	};
}