#include "LayerId.h"
#include <iostream>

namespace rendering
{
	const LayerId LayerId::GROUND_LAYER{ 1 };
	const LayerId LayerId::ABOVE_GROUND_LAYER{ 2 };
	const LayerId LayerId::DEBUG_ABOVE_GROUND{ 10 };
	const LayerId LayerId::DEBUG_FIRST_PLAN{ UINT32_MAX - 2 };
	const LayerId LayerId::DEBUG_SCREENSPACE{ UINT32_MAX -1 };
	const LayerId LayerId::EDITOR_SELECTION{ UINT32_MAX };
}