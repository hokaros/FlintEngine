#include "NavmeshAgent.h"

#include <Navigation/NavmeshPathfinder.h>

namespace Navigation
{
    NavmeshAgent::NavmeshAgent(const Navmesh& navmesh, IMovable& movable, float width)
        : m_Navmesh(navmesh)
        , m_Movable(movable)
        , m_Width(width)
    {
    }

    void NavmeshAgent::Update()
    {
        m_PathFollower.Update(m_Movable);
    }

    void NavmeshAgent::SetTarget(const Vector& targetPos)
    {
        NavmeshPath path;
        NavmeshPathfinder::FindPath(m_Navmesh, m_Movable.GetPosition(), targetPos, m_Width, path);
        m_PathFollower.StartPath(std::move(path));
    }
}