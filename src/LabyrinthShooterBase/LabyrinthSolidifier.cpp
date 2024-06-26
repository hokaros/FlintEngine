#include "LabyrinthSolidifier.h"
#include <Components/BoxCollider.h>
#include "Occlusion/OccludableRectangle.h"

DEFINE_COMPONENT(LabyrinthSolidifier);
DEFINE_FIELD(LabyrinthSolidifier, position);
DEFINE_FIELD(LabyrinthSolidifier, wallWidth);
DEFINE_FIELD(LabyrinthSolidifier, wallLength);
DEFINE_FIELD(LabyrinthSolidifier, xCount);
DEFINE_FIELD(LabyrinthSolidifier, yCount);
DEFINE_FIELD(LabyrinthSolidifier, m_WallColor);
DEFINE_FIELD(LabyrinthSolidifier, m_GateColor);
DEFINE_FIELD(LabyrinthSolidifier, shouldChange);
DEFINE_FIELD(LabyrinthSolidifier, changeTime);
DEFINE_FIELD(LabyrinthSolidifier, wallPrefab);

LabyrinthSolidifier::LabyrinthSolidifier(const Vector& pos,
	int wallWidth, int wallLength,
	int xCount, int yCount,
	double changeTime,
	bool shouldChange)
	: position(pos)
	, wallWidth(wallWidth)
	, wallLength(wallLength)
	, xCount(xCount)
	, yCount(yCount)
	, changeTime(changeTime)
	, shouldChange(shouldChange)
{

}

LabyrinthSolidifier::~LabyrinthSolidifier() 
{
	delete[] walls;
	delete[] border;
}

const Labirynt& LabyrinthSolidifier::GetLab() const {
	return *labyrinth;
}

int LabyrinthSolidifier::WallsCount() const {
	return labyrinth->ActiveCount();
}

GameObject** LabyrinthSolidifier::GetWalls() const {
	return walls;
}

int LabyrinthSolidifier::BorderElements() const {
	return borderCount;
}

GameObject** LabyrinthSolidifier::GetBorder() const {
	return border;
}

Vector LabyrinthSolidifier::GetSize() const {
	return LabyrinthSize(wallWidth, wallLength, xCount, yCount);
}

Vector LabyrinthSolidifier::LabyrinthSize(int wallWidth, int wallLength, int xCount, int yCount) {
	return Vector(wallLength * xCount, wallLength * yCount);
}

void LabyrinthSolidifier::PlaceWalls() {
	int nextWall = 0;
	Vector verticalWall(wallWidth, wallLength);
	Vector horizontalWall(wallLength, wallWidth);

	// �ciany pionowe
	for (int y = 0; y < yCount; y++) {
		for (int x = 1; x <= xCount - 1; x++) {
			int index = (xCount - 1) * y + x - 1;
			if (labyrinth->WallAtIndex(index)) {
				// Obecna �ciana
				GameObject* wall = walls[nextWall++];
				wall->FindComponent<BoxCollider>()->SetSize(verticalWall);
				wall->FindComponent<RectangleRenderer>()->SetSize(verticalWall);
				SetWallUpperLeft(*wall, Vector(x * wallLength, y * wallLength) + position);
			}
		}
	}

	int horStart = labyrinth->VerticalCount();
	// �ciany poziome
	for (int y = 1; y <= yCount - 1; y++) {
		for (int x = 0; x < xCount; x++) {
			int index = horStart + xCount * (y - 1) + x;
			if (labyrinth->WallAtIndex(index)) {
				// Obecna �ciana
				GameObject* wall = walls[nextWall++];
				wall->FindComponent<BoxCollider>()->SetSize(horizontalWall);
				wall->FindComponent<RectangleRenderer>()->SetSize(horizontalWall);
				SetWallUpperLeft(*wall, Vector(x * wallLength, y * wallLength) + position);
			}
		}
	}
}

void LabyrinthSolidifier::SetLab(bool* walls) {
	labyrinth->SetWalls(walls);
	PlaceWalls();

	colliderMemory->Refresh(this->walls, labyrinth->ActiveCount());
}

void LabyrinthSolidifier::ChangeLab() {
	labyrinth->ChangeLab();
	PlaceWalls();

	colliderMemory->Refresh(walls, labyrinth->ActiveCount());
}

const ColliderMemory& LabyrinthSolidifier::GetColliderMemory() const {
	return *colliderMemory;
}

GameObject* LabyrinthSolidifier::BuildWall(const Vector& size) {
	return BuildWall(size, m_WallColor);
}

GameObject* LabyrinthSolidifier::BuildWall(const Vector& size, const Rgb8& color) {
	const GameObject* wallPrefabLoaded = wallPrefab.Get();
	FE_ASSERT(wallPrefabLoaded != nullptr, "Couldn't load wall prefab");

	GameObject* wall = GameObject::Instantiate(*wallPrefabLoaded);

	wall->FindComponent<BoxCollider>()->SetSize(size);

	wall->FindComponent<RectangleRenderer>()->SetColor(color);
	wall->FindComponent<RectangleRenderer>()->SetSize(size);

	return wall;
}

void LabyrinthSolidifier::BuildBorder() {
	borderCount = 8;  // 2* przedziurawione �ciany + zwyk�e �ciany + drzwi
	border = new GameObject * [borderCount];

	GameObject** newBorder = BuildGateWall(Direction::EAST);
	memcpy(border, newBorder, sizeof(GameObject*) * 3);
	newBorder = BuildGateWall(Direction::WEST);
	memcpy(border + 3, newBorder, sizeof(GameObject*) * 3);

	Vector nextPos = position;
	Vector elemSize(GetSize().x, wallWidth);
	border[6] = BuildWall(elemSize);
	SetWallUpperLeft(*border[6], nextPos);
	nextPos.y += yCount * wallLength;
	border[7] = BuildWall(elemSize);
	SetWallUpperLeft(*border[7], nextPos);
}

GameObject** LabyrinthSolidifier::BuildGateWall(Direction side) {
	GameObject** w = new GameObject * [3];

	Vector nextPos = position;
	if (side == Direction::WEST) {
		nextPos.x += xCount * wallLength;
	}
	else if (side == Direction::SOUTH) {
		nextPos.y += wallWidth + yCount * wallLength;
	}
	VectorInt exit = labyrinth->GetExit(side);
	
	if (side == Direction::EAST || side == Direction::WEST) {
		Vector elemSize(wallWidth, exit.y * wallLength);
		w[0] = BuildWall(elemSize);
		SetWallUpperLeft(*w[0], nextPos);
		nextPos.y += elemSize.y;

		// Brama
		elemSize = Vector(wallWidth, wallLength);
		w[1] = BuildWall(elemSize, m_GateColor);
		SetWallUpperLeft(*w[1], nextPos);
		nextPos.y += elemSize.y;

		elemSize = Vector(
			wallWidth,
			(yCount - exit.y - 1) * wallLength
		);
		w[2] = BuildWall(elemSize);
		SetWallUpperLeft(*w[2], nextPos);
	}
	else {
		Vector elemSize(exit.x * wallLength, wallWidth);
		w[0] = BuildWall(elemSize);
		SetWallUpperLeft(*w[0], nextPos);
		nextPos.x += elemSize.x;

		// Brama
		elemSize = Vector(wallLength, wallWidth);
		w[1] = BuildWall(elemSize, m_GateColor);
		SetWallUpperLeft(*w[1], nextPos);
		nextPos.x += elemSize.x;

		elemSize = Vector(
			(xCount - exit.x - 1) * wallLength,
			wallWidth
		);
		w[2] = BuildWall(elemSize);
		SetWallUpperLeft(*w[2], nextPos);
	}

	return w;
}

void LabyrinthSolidifier::SetWallUpperLeft(GameObject& wall, const Vector& pos)
{
	const Vector size = wall.FindComponent<BoxCollider>()->GetWorldSize();
	wall.SetWorldPosition(pos + size / 2.f);
}

void LabyrinthSolidifier::Awake()
{
	labyrinth = std::make_unique<Labirynt>(xCount, yCount);
	colliderMemory = std::make_unique<ColliderMemory>(LabyrinthSize(wallWidth, wallLength, xCount, yCount).x + position.x, LabyrinthSize(wallWidth, wallLength, xCount, yCount).y + position.y);

	// Stworzenie �cian
	walls = new GameObject * [labyrinth->ActiveCount()];
	function<void(Destroyable&)> destroyedHandler = [this](Destroyable& source) {OnWallDestroyedChanged(source); };
	for (int i = 0; i < labyrinth->ActiveCount(); i++)
	{
		walls[i] = BuildWall(Vector(wallWidth, wallLength));

		std::unique_ptr<Destroyable> destroyable = std::make_unique<Destroyable>();
		destroyable->SubscribeDestroyed(destroyedHandler);
		walls[i]->AddComponent(std::move(destroyable));

		walls[i]->AddComponent(std::make_unique<Regenerable>(WALL_REGEN));
	}

	PlaceWalls(); // wstawienie �cian w odpowiednie miejsca
	BuildBorder();

	// Skrajne �ciany zawsze widoczne
	for (int i = 0; i < borderCount; i++)
	{
		OccludableRectangle* occludable = border[i]->FindComponent<OccludableRectangle>();
		std::unique_ptr<RectangleRenderer> unoccludable = std::make_unique<RectangleRenderer>();
		unoccludable->SetColor(occludable->GetColor());
		unoccludable->SetSize(occludable->GetSize());

		border[i]->AddComponent(std::move(unoccludable));
		border[i]->RemoveComponent(occludable);
	}

	colliderMemory->Refresh(walls, labyrinth->ActiveCount());
	labyrinth->PrintLab(); // wy�wietlenie w konsoli
}

void LabyrinthSolidifier::Update() {
	if (!shouldChange)
		return;

	timeSinceLastChange += Timer::Main()->GetDeltaTime();
	if (timeSinceLastChange >= changeTime) {
		ChangeLab();
		timeSinceLastChange = 0;
	}
}

void LabyrinthSolidifier::OnWallDestroyedChanged(Destroyable& wall) {
	if (wall.IsDestroyed()) {
		colliderMemory->Free(&wall.GetOwner());
	}
	else {
		colliderMemory->Claim(&wall.GetOwner());
	}
}