#include "OcclusionSystem.h"
#include <Core/Scene.h>
#include <FTL/VectorOperations.h>

DEFINE_COMPONENT(OcclusionSystem);
DEFINE_FIELD(OcclusionSystem, m_PlayerName);
DEFINE_FIELD(OcclusionSystem, m_LabName);

OcclusionSystem* OcclusionSystem::s_Instance = nullptr;

void OcclusionSystem::Awake()
{
	Scene* scene = GetOwner().GetScene();
	m_Player = scene->FindGameObjectByName(m_PlayerName);
	m_Lab = FindLabyrinth();

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
	if (s_Instance == nullptr)
	{
		FE_WARN("No OcclusionSystem but trying to register an IOccludable");
		return;
	}

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
	return true;

	// Wyœwietlanie tylko, jeœli obiekt jest widziany przez obecnego gracza
	if ((go.GetWorldPosition() - m_Player->GetTransformable().GetWorldPosition()).LengthSquared() > PLAYER_SIGHT * PLAYER_SIGHT)
		return false;  // zbyt daleko

	// Sprawdzenie, czy œciana stoi na drodze
	return !m_Lab->GetColliderMemory().Raycast(
		m_Player->GetTransformable().GetWorldPosition(),
		go.GetWorldPosition(),
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

LabyrinthSolidifier* OcclusionSystem::FindLabyrinth() const
{
	Scene* scene = GetOwner().GetScene();
	GameObject* lab_go = scene->FindGameObjectByName(m_LabName);
	if (lab_go == nullptr)
		return nullptr;

	return lab_go->FindComponent<LabyrinthSolidifier>();
}
