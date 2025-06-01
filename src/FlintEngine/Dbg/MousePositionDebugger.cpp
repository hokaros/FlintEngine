#include "MousePositionDebugger.h"

#include <Dbg/GameDebugManager.h>
#include <InputController.h>
#include <sstream>

namespace debug
{
	MousePositionDebugger::MousePositionDebugger(GameDebugManager& debug_manager)
		: m_DebugManager(debug_manager)
	{
	}

	void MousePositionDebugger::Update()
	{
		if (const IInputController* input = IInputController::Main())
		{
			const Vector mouse_pos = input->GetMousePosition();

			std::stringstream ss;
			ss << "(" << mouse_pos.x << "," << mouse_pos.y << ")";
			m_DebugManager.GetOverlayText().UpdateCornerText(ss.str());
		}
	}
}