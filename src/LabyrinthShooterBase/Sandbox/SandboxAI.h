#pragma once
#include <ComponentDefinition.h>
#include <AI/BehaviorTree/Node.h>

#include "AIContext.h"

class SandboxAI
	: public ObjectComponent
{
	DECLARE_COMPONENT();

public:
	virtual void Awake() override;
	virtual void Update() override;

private:
	std::unique_ptr<bt::Node<AIContext>> CreateTestBehaviorTree();
	std::unique_ptr<AIContext> CreateContext(AIPerception& perception);

private:
	std::unique_ptr<bt::Node<AIContext>> m_BehaviorTree = nullptr;

	std::unique_ptr<AIPerception> m_Perception = nullptr;
	std::unique_ptr<AIContext> m_Context = nullptr;
};
