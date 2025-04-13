#include "PlayerControlContext.h"

namespace Controllers
{
	PlayerControlContext::PlayerControlContext(const IInputController& input, PlayerEquipment& equipment)
		: m_Input(input)
		, m_Equipment(equipment)
	{
	}

	const IInputController& PlayerControlContext::GetInput() const
	{
		return m_Input;
	}

	PlayerEquipment& PlayerControlContext::GetEquipment()
	{
		return m_Equipment;
	}
}