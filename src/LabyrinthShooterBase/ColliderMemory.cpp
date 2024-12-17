#include "ColliderMemory.h"
#include <Window.h>
#include <Draw.h>
#include <Components/RectangleRenderer.h>

ColliderMemory::ColliderMemory(size_t width, size_t height)
	: width(width), height(height)
{

	memory = new GameObject** [width];

	for (size_t x = 0; x < width; x++)
	{
		memory[x] = new GameObject*[height];

		for (size_t y = 0; y < height; y++)
		{
			memory[x][y] = nullptr;
		}
	}
}

ColliderMemory::~ColliderMemory()
{
	for (size_t x = 0; x < width; x++)
	{
		delete[] memory[x];
	}

	delete[] memory;
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
	for (size_t x = 0; x < width; x++) {
		for (size_t y = 0; y < height; y++)
		{
			memory[x][y] = nullptr;
		}
	}
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
		if (pixel.x >= 0 && pixel.x < width
			&& pixel.y >= 0 && pixel.y < height)
		{

			memory[pixel.x][pixel.y] = occupier;
		}
	}
}

bool ColliderMemory::IsOccupied(const VectorInt& point) const
{
	if (point.x < 0 || point.x >= width || point.y < 0 || point.y >= height)
		return false;

	return memory[point.x][point.y] != nullptr && memory[point.x][point.y]->IsEnabled();
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

		if (IsOccupied(point) && (ignore == nullptr || memory[point.x][point.y] != ignore))
			return true;

		curr += dPosPart;
	}

	return false;
}

size_t ColliderMemory::GetWidth() const
{
	return width;
}

size_t ColliderMemory::GetHeight() const
{
	return height;
}