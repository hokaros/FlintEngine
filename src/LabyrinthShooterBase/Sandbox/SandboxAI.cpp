#include "SandboxAI.h"

#include "MoveBehavior.h"
#include "SetRandomMoveTargetBehavior.h"
#include "WaitBehavior.h"

#include <Core/GameObject.h>
#include <AI/BehaviorTree/Sequence.h>
#include <AI/BehaviorTree/BehaviorTreeBuilder.h>

DEFINE_COMPONENT(SandboxAI);

void SandboxAI::Awake()
{
	m_Perception = std::make_unique<AIPerception>(GetOwner());
	m_Context = CreateContext(*m_Perception);

	// TODO: get tree from file
	m_BehaviorTree = CreateTestBehaviorTree();
}

void SandboxAI::Update()
{
	m_BehaviorTree->Run(*m_Context);
}

std::unique_ptr<bt::Node<AIContext>> SandboxAI::CreateTestBehaviorTree()
{
	return bt::TreeBuilder::Composite<bt::Sequence<AIContext>>()
		->Child(std::make_unique<SetRandomMoveTargetBehavior>())
		.Child(std::make_unique<bt::MoveBehavior>())
		.Child(std::make_unique<WaitBehavior>(1.f))
		.Finalize();
}

std::unique_ptr<AIContext> SandboxAI::CreateContext(AIPerception& perception)
{
	ConstantMover* mover = GetOwner().FindComponent<ConstantMover>();
	FE_ASSERT(mover != nullptr, "No ConstantMover in AI");

	return std::make_unique<AIContext>(perception, *mover);
}
