#pragma once
#include "../FlatEngine/ComponentDefinition.h"
#include "../FlatEngine/imgui/imgui.h"

class ComponentEditor
{
    // Edytor komponentu nie musi by� szablonem, poniewa�:
    // Component->GetRTC()
    // Na podstawie RTC pobieramy ComponentDefinition z ComponentDefinitionManagera
    // ComponentDefinition zawiera wszystkie fieldy oraz nazw� komponentu
    //
    // ComponentFieldDefinition->GetValueRTC() aby stworzy� odpowiednie pole w edytorze
    // ComponentFieldDefinition->SetFieldValue(, void* value)
    // ComponentFieldDefinition->GetFieldValue(, void* value)
public:
    ComponentEditor(ObjectComponent& component);
    void Render();

private:
    ObjectComponent& m_Component;
    ComponentDefinition& m_ComponentDefinition;
};

