#pragma once
#include "GameObjectSerializer.h"
#include "PrefabInstance.h"

struct PrefabInstanceStringDesc
{
	std::string m_PrefabPath;
	std::map<std::string, std::string> m_OverridenParams;
	// TODO: components
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
};

