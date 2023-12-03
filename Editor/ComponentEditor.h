#pragma once
#include "../FlatEngine/ComponentDefinition.h"
#include "../FlatEngine/imgui/imgui.h"

class ComponentEditor
{
public:
    ComponentEditor(ObjectComponent& component, size_t index_in_game_object);
    void Render();

private:
    ObjectComponent& m_Component;
    ComponentDefinition& m_ComponentDefinition;
    size_t m_IndexInGameObject;
};

