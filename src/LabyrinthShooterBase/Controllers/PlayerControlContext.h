#pragma once

class IInputController;
class PlayerEquipment;

namespace Controllers
{
	class PlayerControlContext
	{
	public:
		PlayerControlContext(const IInputController& input, PlayerEquipment& equipment);

		const IInputController& GetInput() const;
		PlayerEquipment& GetEquipment();

	private:
		const IInputController& m_Input;
		PlayerEquipment& m_Equipment;
	};
}