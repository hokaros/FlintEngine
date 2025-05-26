#include "ClickPlayerController.h"

#include <Core/Scene.h>
#include <Core/GameBase.h>

DEFINE_COMPONENT(ClickPlayerController);

void ClickPlayerController::Awake()
{
	input = IInputController::Main();
	equipment = GetOwner().FindComponent<PlayerEquipment>();
	mover = GetOwner().FindComponent<ConstantMover>();

	const float collider_extent = GetOwner().FindComponent<BoxCollider>()->GetWorldSize().Length();

	FE_ASSERT(input != nullptr, "No input");
	FE_ASSERT(equipment != nullptr, "No equipment");
	FE_ASSERT(mover != nullptr, "No movement component");

	m_NavmeshAgent = std::make_unique<Navigation::NavmeshAgent>(GetOwner().GetScene()->GetNavmesh(), *mover, collider_extent / 2.0f);
}

void ClickPlayerController::Update()
{
	ProcessMovement();

	Controllers::PlayerControlContext context(*input, *equipment);
	m_ActionsController.Update(context);

	m_NavmeshAgent->Update();
}

void ClickPlayerController::ProcessMovement()
{
	SceneRenderer* sceneRenderer = GameBase::GetCurrent()->GetSceneRenderer();
	if (sceneRenderer == nullptr)
		return;

	if (input->MouseButtonPressedThisFrame(MouseButton::Right))
	{
		const Vector mousePos = input->GetMousePosition();

		const Vector worldPos = sceneRenderer->ScreenSpaceToWorldSpace(mousePos);
		m_NavmeshAgent->SetTarget(worldPos);
	}
}
