#include "OcclusionSystem.h"
#include <Scene.h>
#include <ftl.h>

DEFINE_COMPONENT(OcclusionSystem);
DEFINE_FIELD(OcclusionSystem, m_PlayerName);
DEFINE_FIELD(OcclusionSystem, m_LabName);

OcclusionSystem* OcclusionSystem::s_Instance = nullptr;

void OcclusionSystem::Awake()
{
	Scene* scene = GetOwner().GetScene();
	m_Player = scene->FindGameObjectByName(m_PlayerName);
	m_Lab = scene->FindGameObjectByName(m_LabName)->FindComponent<LabyrinthSolidifier>();

	s_Instance = this;
}

void OcclusionSystem::Update()
{
	for (IOccludable* occludable : m_Occludables)
	{
		GameObject& owner_object = occludable->GetOwnerObject();

		if (ShouldBeRendered(owner_object))
		{
			occludable->EnableRendering();
		}
		else
		{
			occludable->DisableRendering();
		}
	}
}

void OcclusionSystem::OnDestroy()
{
	if (s_Instance == this)
	{
		s_Instance = nullptr;
	}
}

void OcclusionSystem::RegisterOccludable(IOccludable& occludable)
{
	FE_ASSERT(s_Instance != nullptr, "No instance"); // TODO: create on demand

	s_Instance->RegisterOccludableImpl(occludable);
}

void OcclusionSystem::UnregisterOccludable(IOccludable& occludable)
{
	if (s_Instance == nullptr)
		return;

	s_Instance->UnregisterOccludableImpl(occludable);
}

OcclusionSystem* OcclusionSystem::GetInstance()
{
	return s_Instance;
}

bool OcclusionSystem::ShouldBeRendered(GameObject& go) const
{
	// Wyœwietlanie tylko, jeœli obiekt jest widziany przez obecnego gracza
	if ((go.GetPosition() - m_Player->GetPosition()).LengthSquared() > PLAYER_SIGHT * PLAYER_SIGHT)
		return false;  // zbyt daleko

	// Sprawdzenie, czy œciana stoi na drodze
	return !m_Lab->GetColliderMemory().Raycast(
		m_Player->GetMiddle(),
		go.GetMiddle(),
		&go
	);
}

void OcclusionSystem::RegisterOccludableImpl(IOccludable& occludable)
{
	m_Occludables.push_back(&occludable);
}

void OcclusionSystem::UnregisterOccludableImpl(IOccludable& occludable)
{
	ftl::vector_remove(m_Occludables, &occludable);
}
