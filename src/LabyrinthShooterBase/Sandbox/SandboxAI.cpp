#include "SandboxAI.h"
#include "MoveBehavior.h"

#include <Core/GameObject.h>

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
	return std::make_unique<bt::MoveBehavior>(context);
}

std::unique_ptr<AIContext> SandboxAI::CreateContext(AIPerception& perception)
{
	ConstantMover* mover = GetOwner().FindComponent<ConstantMover>();
	FE_ASSERT(mover != nullptr, "No ConstantMover in AI");

	return std::make_unique<AIContext>(perception, *mover);
}
