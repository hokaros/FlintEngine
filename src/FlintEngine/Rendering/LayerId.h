#pragma once
#include <SimpleTypes.h>

namespace rendering
{
	struct LayerId
	{
	public:
		uint id;

		constexpr explicit LayerId(uint id)
			: id(id)
		{}

		static const LayerId GROUND_LAYER;
		static const LayerId ABOVE_GROUND_LAYER;
		static const LayerId EDITOR_SELECTION;
		static const LayerId DEBUG_ABOVE_GROUND;
		static const LayerId DEBUG_FIRST_PLAN;
		static const LayerId DEBUG_SCREENSPACE;
	};
}