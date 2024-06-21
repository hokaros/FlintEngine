#pragma once
#include <ComponentDefinition.h>
#include <Core/GameObject.h>
#include "LabyrinthSolidifier.h"

class IOccludable
{
public:
	virtual GameObject& GetOwnerObject() = 0;
	virtual void EnableRendering() = 0;
	virtual void DisableRendering() = 0;

	virtual ~IOccludable() = default;
};

class OcclusionSystem
	: public ObjectComponent
{
	DECLARE_COMPONENT();
public:
	OcclusionSystem() = default;

	virtual void Awake() override;
	virtual void Update() override;
	virtual void OnDestroy() override;

	static void RegisterOccludable(IOccludable& occludable);
	static void UnregisterOccludable(IOccludable& occludable);
	static OcclusionSystem* GetInstance(); // Can return null if there are no Occludable components registered

private:
	bool ShouldBeRendered(GameObject& game_object) const;

	void RegisterOccludableImpl(IOccludable& occludable);
	void UnregisterOccludableImpl(IOccludable& occludable);

	LabyrinthSolidifier* FindLabyrinth() const;

private:
	std::string m_PlayerName = "Player";
	DECLARE_FIELD(m_PlayerName);

	std::string m_LabName = "Labyrinth";
	DECLARE_FIELD(m_LabName);

	GameObject* m_Player = nullptr;
	LabyrinthSolidifier* m_Lab = nullptr;

	std::vector<IOccludable*> m_Occludables;

	static OcclusionSystem* s_Instance;
	static constexpr float PLAYER_SIGHT = 400;
};