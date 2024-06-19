#include "PrefabInstanceSerializer.h"

std::unique_ptr<PrefabInstanceStringDesc> PrefabInstanceSerializer::Serialize(const PrefabInstance& prefab_instance)
{
	std::unique_ptr<PrefabInstanceStringDesc> desc = std::make_unique<PrefabInstanceStringDesc>();

	desc->m_PrefabPath = prefab_instance.GetPrefabPath();
	SerializeParamOverrides(prefab_instance, *desc);
	SerializeAdditionalComponents(prefab_instance, *desc);

	return desc;
}

std::unique_ptr<PrefabInstance> PrefabInstanceSerializer::Deserialize(const PrefabInstanceStringDesc& desc)
{
	std::unique_ptr<PrefabInstance> prefab_instance = std::make_unique<PrefabInstance>(desc.m_PrefabPath);

	DeserializeParamOverrides(desc, *prefab_instance);
	DeserializeAdditionalComponents(desc, *prefab_instance);

	return prefab_instance;
}

void PrefabInstanceSerializer::SerializeParamOverrides(const PrefabInstance& prefab_instance, PrefabInstanceStringDesc& desc)
{
	if (prefab_instance.GetNameOverride().has_value())
	{
		desc.m_OverridenParams[s_NameKey] = prefab_instance.GetNameOverride().value();
	}
	if (prefab_instance.GetPositionOverride().has_value())
	{
		desc.m_OverridenParams[s_PositionKey] = SerializableTypeInterface<Vector>::ToString(prefab_instance.GetPositionOverride().value());
	}
	if (prefab_instance.GetSizeOverride().has_value())
	{
		desc.m_OverridenParams[s_SizeKey] = SerializableTypeInterface<Vector>::ToString(prefab_instance.GetSizeOverride().value());
	}
}

void PrefabInstanceSerializer::DeserializeParamOverrides(const PrefabInstanceStringDesc& desc, PrefabInstance& prefab_instance)
{
	for (auto& entry : desc.m_OverridenParams)
	{
		const std::string& key = entry.first;
		const std::string& value = entry.second;

		if (key == s_NameKey)
		{
			prefab_instance.SetName(value);
		}
		else if (key == s_PositionKey)
		{
			Vector pos;
			SerializableTypeInterface<Vector>::ParseString(value, pos);
			prefab_instance.GetTransformable().SetWorldPosition(pos);
		}
		else if (key == s_SizeKey)
		{
			Vector size;
			SerializableTypeInterface<Vector>::ParseString(value, size);
			prefab_instance.GetTransformable().SetWorldScale(size);
		}
		else
		{
			FE_DATA_ERROR("Unknown PrefabInstance param key: %s", key.c_str());
		}
	}
}

void PrefabInstanceSerializer::SerializeAdditionalComponents(const PrefabInstance& prefab_instance, PrefabInstanceStringDesc& desc)
{
	for (const ObjectComponent* comp : prefab_instance.GetAdditionalComponents())
	{
		desc.m_AdditionalComponents.push_back(ComponentSerializer::SerializeComponent(*comp));
	}
}

void PrefabInstanceSerializer::DeserializeAdditionalComponents(const PrefabInstanceStringDesc& desc, PrefabInstance& prefab_instance)
{
	for (const std::unique_ptr<ComponentStringDesc>& comp_desc : desc.m_AdditionalComponents)
	{
		prefab_instance.Serializable_AddComponent(ComponentSerializer::DeserializeComponent(*comp_desc));
	}
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
