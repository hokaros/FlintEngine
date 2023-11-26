#pragma once
#include "../FlatEngine/ComponentDefinition.h"
#include "../FlatEngine/imgui/imgui.h"

class ComponentEditor
{
public:
    ComponentEditor(ObjectComponent& component);
    void Render();

private:
    ObjectComponent& m_Component;
    ComponentDefinition& m_ComponentDefinition;
};

