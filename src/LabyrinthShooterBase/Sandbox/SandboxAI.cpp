#include "SandboxAI.h"

#include "MoveBehavior.h"
#include "PlayerMoveTargetSetBehavior.h"

#include <Core/GameObject.h>
#include <AI/BehaviorTree/Sequence.h>
#include <AI/BehaviorTree/BehaviorTreeBuilder.h>

DEFINE_COMPONENT(SandboxAI);

void SandboxAI::Awake()
{
	m_Perception = std::make_unique<AIPerception>(GetOwner());
	m_Context = CreateContext(*m_Perception);

	// TODO: get tree from file
	m_BehaviorTree = CreateTestBehaviorTree(*m_Context);
}

void SandboxAI::Update()
{
	m_BehaviorTree->Run();
}

std::unique_ptr<bt::Node> SandboxAI::CreateTestBehaviorTree(AIContext& context)
{
	return bt::TreeBuilder::Composite<bt::Sequence>()
		->Child(std::make_unique<PlayerMoveTargetSetBehavior>(context))
		.Child(std::make_unique<bt::MoveBehavior>(context))
		.Finalize();
}

std::unique_ptr<AIContext> SandboxAI::CreateContext(AIPerception& perception)
{
	ConstantMover* mover = GetOwner().FindComponent<ConstantMover>();
	FE_ASSERT(mover != nullptr, "No ConstantMover in AI");

	return std::make_unique<AIContext>(perception, *mover);
}
