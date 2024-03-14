#include "PrefabInstanceSerializer.h"

std::unique_ptr<PrefabInstanceStringDesc> PrefabInstanceSerializer::Serialize(const PrefabInstance& prefab_instance)
{
	FE_ASSERT(false, "Unimplemented");
	return std::make_unique<PrefabInstanceStringDesc>();
}

std::unique_ptr<PrefabInstance> PrefabInstanceSerializer::Deserialize(const PrefabInstanceStringDesc& desc)
{
	FE_ASSERT(false, "Unimplemented");
	return std::unique_ptr<PrefabInstance>();
}


PrefabInstanceStringDescEndpoint::PrefabInstanceStringDescEndpoint(std::unique_ptr<PrefabInstanceStringDesc> desc)
	: m_Desc(std::move(desc))
{
}

PrefabInstanceStringDesc& PrefabInstanceStringDescEndpoint::GetDesc()
{
	return *m_Desc;
}

const PrefabInstanceStringDesc& PrefabInstanceStringDescEndpoint::GetDesc() const
{
	return *m_Desc;
}

EditableGameObjectType PrefabInstanceStringDescEndpoint::GetType() const
{
	return EditableGameObjectType::PrefabInstance;
}
