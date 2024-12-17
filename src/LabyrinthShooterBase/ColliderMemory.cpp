#include "ColliderMemory.h"
#include <Window.h>
#include <Draw.h>
#include <Components/RectangleRenderer.h>

ColliderMemory::ColliderMemory(size_t width, size_t height)
	: m_Memory(width, height)
{
	m_Memory.set_all(nullptr);
}

void ColliderMemory::Refresh(ftl::span<GameObject*> objects)
{
	Clear();

	for (GameObject* object : objects)
	{
		if (object->IsEnabled())
		{
			Claim(object);
		}
	}
}

void ColliderMemory::Clear()
{
	m_Memory.set_all(nullptr);
}

void ColliderMemory::Claim(GameObject* collider)
{
	SetForCollider(collider, collider);
}


void ColliderMemory::Free(GameObject* collider)
{
	SetForCollider(collider, nullptr);
}

void ColliderMemory::SetForCollider(GameObject* collider, GameObject* occupier)
{
	RectangleRenderer* renderer = collider->FindComponent<RectangleRenderer>();
	if (renderer == nullptr)
		return;

	std::vector<VectorInt> pixels = renderer->GetPixels();

	for (VectorInt pixel : pixels)
	{
		if (pixel.x >= 0 && pixel.x < m_Memory.width()
			&& pixel.y >= 0 && pixel.y < m_Memory.height())
		{
			m_Memory.at(pixel.x, pixel.y) = occupier;
		}
	}
}

bool ColliderMemory::IsOccupied(const VectorInt& point) const
{
	if (point.x < 0 || point.x >= m_Memory.width() || point.y < 0 || point.y >= m_Memory.height())
		return false;

	return m_Memory.at(point.x, point.y) != nullptr && m_Memory.at(point.x, point.y)->IsEnabled();
}

bool ColliderMemory::Raycast(const VectorInt& start, const VectorInt& end, GameObject* ignore) const
{
	Vector dPos = (Vector)end - start;
	Vector dPosPart = dPos;
	dPosPart.Normalize();

	Vector curr;

	while (curr.LengthSquared() < dPos.LengthSquared())
	{
		VectorInt point(start + (VectorInt)curr);

		if (IsOccupied(point) && (ignore == nullptr || m_Memory.at(point.x, point.y) != ignore))
			return true;

		curr += dPosPart;
	}

	return false;
}

size_t ColliderMemory::GetWidth() const
{
	return m_Memory.width();
}

size_t ColliderMemory::GetHeight() const
{
	return m_Memory.height();
}