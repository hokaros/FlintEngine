#pragma once
#include "GameObjectSerializer.h"
#include "PrefabInstance.h"

struct PrefabInstanceStringDesc
{
	std::string m_PrefabPath;
	std::map<std::string, std::string> m_OverridenParams;
	// TODO: additional children
	std::vector<std::unique_ptr<ComponentStringDesc>> m_AdditionalComponents;
	// TODO: component modifications
	// TODO: removed components
};

class PrefabInstanceStringDescEndpoint
	: public GameObjectStringDescProxy
{
public:
	PrefabInstanceStringDescEndpoint(std::unique_ptr<PrefabInstanceStringDesc> desc);

	PrefabInstanceStringDesc& GetDesc();
	const PrefabInstanceStringDesc& GetDesc() const;

	virtual EditableGameObjectType GetType() const override;

private:
	std::unique_ptr<PrefabInstanceStringDesc> m_Desc;
};

class PrefabInstanceSerializer
{
public:
	static std::unique_ptr<PrefabInstanceStringDesc> Serialize(const PrefabInstance& prefab_instance);
	static std::unique_ptr<PrefabInstance> Deserialize(const PrefabInstanceStringDesc& desc);

public:
	static constexpr const char* s_NameKey = "name";
	static constexpr const char* s_PositionKey = "pos";
	static constexpr const char* s_SizeKey = "size";
	static constexpr const char* s_PrefabKey = "Prefab";

private:
	static void SerializeParamOverrides(const PrefabInstance& prefab_instance, PrefabInstanceStringDesc& desc);
	static void DeserializeParamOverrides(const PrefabInstanceStringDesc& desc, PrefabInstance& prefab_instance);

	static void SerializeAdditionalComponents(const PrefabInstance& prefab_instance, PrefabInstanceStringDesc& desc);
	static void DeserializeAdditionalComponents(const PrefabInstanceStringDesc& desc, PrefabInstance& prefab_instance);
};

