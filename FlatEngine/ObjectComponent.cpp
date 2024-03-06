#include "ObjectComponent.h"
#include "ComponentDefinition.h"

GameObject& ObjectComponent::GetOwner() const 
{
	return *m_GameObject;
}

std::unique_ptr<ObjectComponent> ObjectComponent::Copy()
{
	ComponentDefinition* comp_def = ComponentDefinitionManager::GetInstance().GetDefinitionFromTypeCode(GetTypeCode());
	return comp_def->CopyComponent(*this);
}

void ObjectComponent::SetOwner(GameObject* owner)
{
	m_GameObject = owner;
}
