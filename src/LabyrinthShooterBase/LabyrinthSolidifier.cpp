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

	// Œciany pionowe
	for (int y = 0; y < yCount; y++) {
		for (int x = 1; x <= xCount - 1; x++) {
			int index = (xCount - 1) * y + x - 1;
			if (labyrinth->WallAtIndex(index)) {
				// Obecna œciana
				GameObject* wall = walls[nextWall++];
				wall->SetSize(verticalWall);
				wall->FindComponent<BoxCollider>()->SetSize(verticalWall);
				wall->SetPosition(Vector(x * wallLength, y * wallLength) + position);
			}
		}
	}

	int horStart = labyrinth->VerticalCount();
	// Œciany poziome
	for (int y = 1; y <= yCount - 1; y++) {
		for (int x = 0; x < xCount; x++) {
			int index = horStart + xCount * (y - 1) + x;
			if (labyrinth->WallAtIndex(index)) {
				// Obecna œciana
				GameObject* wall = walls[nextWall++];
				wall->SetSize(horizontalWall);
				wall->FindComponent<BoxCollider>()->SetSize(horizontalWall);
				wall->SetPosition(Vector(x * wallLength, y * wallLength) + position);
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

	wall->SetSize(size);
	wall->FindComponent<BoxCollider>()->SetSize(size);

	wall->FindComponent<RectangleRenderer>()->SetColor(color);

	return wall;
}

void LabyrinthSolidifier::BuildBorder() {
	borderCount = 8;  // 2* przedziurawione œciany + zwyk³e œciany + drzwi
	border = new GameObject * [borderCount];

	GameObject** newBorder = BuildGateWall(Direction::EAST);
	memcpy(border, newBorder, sizeof(GameObject*) * 3);
	newBorder = BuildGateWall(Direction::WEST);
	memcpy(border + 3, newBorder, sizeof(GameObject*) * 3);

	Vector nextPos = position;
	Vector elemSize(GetSize().x, wallWidth);
	border[6] = BuildWall(elemSize);
	border[6]->SetPosition(nextPos);
	nextPos.y += yCount * wallLength;
	border[7] = BuildWall(elemSize);
	border[7]->SetPosition(nextPos);
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
		w[0]->SetPosition(nextPos);
		nextPos.y += elemSize.y;

		// Brama
		elemSize = Vector(wallWidth, wallLength);
		w[1] = BuildWall(elemSize, m_GateColor);
		w[1]->SetPosition(nextPos);
		nextPos.y += elemSize.y;

		elemSize = Vector(
			wallWidth,
			(yCount - exit.y - 1) * wallLength
		);
		w[2] = BuildWall(elemSize);
		w[2]->SetPosition(nextPos);
	}
	else {
		Vector elemSize(exit.x * wallLength, wallWidth);
		w[0] = BuildWall(elemSize);
		w[0]->SetPosition(nextPos);
		nextPos.x += elemSize.x;

		// Brama
		elemSize = Vector(wallLength, wallWidth);
		w[1] = BuildWall(elemSize, m_GateColor);
		w[1]->SetPosition(nextPos);
		nextPos.x += elemSize.x;

		elemSize = Vector(
			(xCount - exit.x - 1) * wallLength,
			wallWidth
		);
		w[2] = BuildWall(elemSize);
		w[2]->SetPosition(nextPos);
	}

	return w;
}

void LabyrinthSolidifier::Awake()
{
	labyrinth = std::make_unique<Labirynt>(xCount, yCount);
	colliderMemory = std::make_unique<ColliderMemory>(LabyrinthSize(wallWidth, wallLength, xCount, yCount).x + position.x, LabyrinthSize(wallWidth, wallLength, xCount, yCount).y + position.y);

	// Stworzenie œcian
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

	PlaceWalls(); // wstawienie œcian w odpowiednie miejsca
	BuildBorder();

	// Skrajne œciany zawsze widoczne
	for (int i = 0; i < borderCount; i++)
	{
		OccludableRectangle* occludable = border[i]->FindComponent<OccludableRectangle>();
		std::unique_ptr<RectangleRenderer> unoccludable = std::make_unique<RectangleRenderer>();
		unoccludable->SetColor(occludable->GetColor());

		border[i]->AddComponent(std::move(unoccludable));
		border[i]->RemoveComponent(occludable);
	}

	colliderMemory->Refresh(walls, labyrinth->ActiveCount());
	labyrinth->PrintLab(); // wyœwietlenie w konsoli
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