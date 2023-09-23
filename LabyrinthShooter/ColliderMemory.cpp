#include "ColliderMemory.h"
#include "Window.h"
#include "Draw.h"

ColliderMemory::ColliderMemory(size_t width, size_t height)
	: width(width), height(height) {

	memory = new GameObject** [width];

	for (size_t x = 0; x < width; x++) {
		memory[x] = new GameObject*[height];

		for (size_t y = 0; y < height; y++) {
			memory[x][y] = NULL;
		}
	}
}

ColliderMemory::~ColliderMemory() {
	for (size_t x = 0; x < width; x++) {
			delete[] memory[x];
	}

	delete[] memory;
}

void ColliderMemory::Refresh(GameObject** objects, size_t count) {
	Clear();

	for (size_t i = 0; i < count; i++) {
		if (!objects[i]->IsDestroyed()) {
			Claim(objects[i]);
		}
	}
}

void ColliderMemory::Clear() {
	for (size_t x = 0; x < width; x++) {
		for (size_t y = 0; y < height; y++) {
			memory[x][y] = NULL;
		}
	}
}

void ColliderMemory::Claim(GameObject* collider) {
	SetForCollider(collider, collider);
}


void ColliderMemory::Free(GameObject* collider) {
	SetForCollider(collider, NULL);
}

void ColliderMemory::SetForCollider(GameObject* collider, GameObject* occupier) {
	std::vector<VectorInt>* pixels = collider->GetPixels();

	for (VectorInt pixel : *pixels) {
		if (pixel.x >= 0 && pixel.x < width
			&& pixel.y >= 0 && pixel.y < height) {

			memory[pixel.x][pixel.y] = occupier;
		}
	}
}

bool ColliderMemory::IsOccupied(const VectorInt& point) const {
	if (point.x < 0 || point.x >= width || point.y < 0 || point.y >= height)
		return false;

	return memory[point.x][point.y] != NULL && memory[point.x][point.y]->IsEnabled();
}

bool ColliderMemory::Raycast(const VectorInt& start, const VectorInt& end, GameObject* ignore) const {
	Vector dPos = (Vector)end - start;
	Vector dPosPart = dPos;
	dPosPart.Normalize();

	Vector curr;

	while (curr.LengthSquared() < dPos.LengthSquared()) {
		VectorInt point(start + (VectorInt)curr);

		if (IsOccupied(point) && (ignore == NULL || memory[point.x][point.y] != ignore))
			return true;

		curr += dPosPart;
	}

	return false;
}

size_t ColliderMemory::GetWidth() const {
	return width;
}

size_t ColliderMemory::GetHeight() const {
	return height;
}