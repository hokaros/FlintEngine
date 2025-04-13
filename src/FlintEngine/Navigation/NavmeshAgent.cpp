#include "NavmeshAgent.h"

#include <Navigation/NavmeshPathfinder.h>

namespace Navigation
{
    NavmeshAgent::NavmeshAgent(const Navmesh& navmesh, IMovable& movable)
        : m_Navmesh(navmesh)
        , m_Movable(movable)
    {
    }

    void NavmeshAgent::Update()
    {
        m_PathFollower.Update(m_Movable);
    }

    void NavmeshAgent::SetTarget(const Vector& targetPos)
    {
        NavmeshPath path;
        NavmeshPathfinder::FindPath(m_Navmesh, m_Movable.GetPosition(), targetPos, path);
        m_PathFollower.StartPath(std::move(path));
    }
}