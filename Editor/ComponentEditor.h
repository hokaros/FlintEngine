#pragma once
#include "../FlatEngine/ComponentDefinition.h"
#include "../FlatEngine/imgui/imgui.h"

class ComponentEditor
{
    // Edytor komponentu nie musi byæ szablonem, poniewa¿:
    // Component->GetRTC()
    // Na podstawie RTC pobieramy ComponentDefinition z ComponentDefinitionManagera
    // ComponentDefinition zawiera wszystkie fieldy oraz nazwê komponentu
    //
    // ComponentFieldDefinition->GetValueRTC() aby stworzyæ odpowiednie pole w edytorze
    // ComponentFieldDefinition->SetFieldValue(, void* value)
    // ComponentFieldDefinition->GetFieldValue(, void* value)
public:
    ComponentEditor(ObjectComponent& component);
    void Render();

private:
    ObjectComponent& m_Component;
    ComponentDefinition& m_ComponentDefinition;
};

