#include "LabyrinthSolidifier.h"

LabyrinthSolidifier::LabyrinthSolidifier(const Vector& pos,
	int wallWidth, int wallLength,
	int xCount, int yCount,
	const std::list<GameObject*>& allObjects,
	double changeTime,
	bool shouldChange)
	: position(pos), wallWidth(wallWidth), wallLength(wallLength),
	xCount(xCount), yCount(yCount),
	labyrinth(xCount, yCount), allObjects(allObjects),
	changeTime(changeTime),
	shouldChange(shouldChange),
	colliderMemory(LabyrinthSize(wallWidth, wallLength, xCount, yCount).x + pos.x, LabyrinthSize(wallWidth, wallLength, xCount, yCount).y + pos.y)
	{

	if (Window::Main() != NULL) {
		SDL_Surface* screen = Window::Main()->GetScreen();
		wallColor = SDL_MapRGB(screen->format, 0x00, 0x00, 0xAA);
		gateColor = SDL_MapRGB(screen->format, 0x00, 0xCC, 0xAA);
	}

	// Stworzenie œcian
	walls = new GameObject * [labyrinth.ActiveCount()];
	function<void(GameObject*)> destroyedHandler = [this](GameObject* source) {OnWallDestroyedChanged(source); };
	for (int i = 0; i < labyrinth.ActiveCount(); i++) {
		walls[i] = BuildWall(Vector(wallWidth, wallLength));
		walls[i]->isDestroyable = true;
		walls[i]->SubscribeDestroyed(destroyedHandler);
	}

	PlaceWalls(); // wstawienie œcian w odpowiednie miejsca
	BuildBorder();

	// Skrajne œciany zawsze widoczne
	for (int i = 0; i < borderCount; i++) {
		border[i]->renderUnseen = true;
	}

	colliderMemory.Refresh(walls, labyrinth.ActiveCount());
	labyrinth.PrintLab(); // wyœwietlenie w konsoli
}

IUpdateable* LabyrinthSolidifier::Copy() {
	return new LabyrinthSolidifier(*this);
}

LabyrinthSolidifier::~LabyrinthSolidifier() {
	for (int i = 0; i < labyrinth.ActiveCount(); i++) {
		delete walls[i];
	}
	delete[] walls;

	for (int i = 0; i < borderCount; i++) {
		delete border[i];
	}
	delete[] border;
}

const Labirynt& LabyrinthSolidifier::GetLab() const {
	return labyrinth;
}

int LabyrinthSolidifier::WallsCount() const {
	return labyrinth.ActiveCount();
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
			if (labyrinth[index]) {
				// Obecna œciana
				GameObject* wall = walls[nextWall++];
				wall->SetSize(verticalWall);
				wall->SetPosition(Vector(x * wallLength, y * wallLength) + position);
			}
		}
	}

	int horStart = labyrinth.VerticalCount();
	// Œciany poziome
	for (int y = 1; y <= yCount - 1; y++) {
		for (int x = 0; x < xCount; x++) {
			int index = horStart + xCount * (y - 1) + x;
			if (labyrinth[index]) {
				// Obecna œciana
				GameObject* wall = walls[nextWall++];
				wall->SetSize(horizontalWall);
				wall->SetPosition(Vector(x * wallLength, y * wallLength) + position);
			}
		}
	}
}

void LabyrinthSolidifier::SetLab(bool* walls) {
	labyrinth.SetWalls(walls);
	PlaceWalls();

	colliderMemory.Refresh(this->walls, labyrinth.ActiveCount());
}

void LabyrinthSolidifier::ChangeLab() {
	labyrinth.ChangeLab();
	PlaceWalls();

	colliderMemory.Refresh(walls, labyrinth.ActiveCount());

	if (onChanged)
		onChanged(labyrinth.getWalls());
}

const ColliderMemory& LabyrinthSolidifier::GetColliderMemory() const {
	return colliderMemory;
}

GameObject* LabyrinthSolidifier::BuildWall(const Vector& size) {
	return BuildWall(size, wallColor);
}

GameObject* LabyrinthSolidifier::BuildWall(const Vector& size, int color) {
	GameObject* wall = new GameObject(size, allObjects);

	wall->AddComponent(new Regenerable(*wall, WALL_REGEN));
	wall->isStatic = true;

	if (Window::Main() != NULL)
		wall->SetRenderer(new RectangleRenderer(*wall, Window::Main()->GetScreen(), color, color));

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
	VectorInt exit = labyrinth.GetExit(side);
	
	if (side == Direction::EAST || side == Direction::WEST) {
		Vector elemSize(wallWidth, exit.y * wallLength);
		w[0] = BuildWall(elemSize);
		w[0]->SetPosition(nextPos);
		nextPos.y += elemSize.y;

		// Brama
		elemSize = Vector(wallWidth, wallLength);
		w[1] = BuildWall(elemSize, gateColor);
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
		w[1] = BuildWall(elemSize, gateColor);
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

void LabyrinthSolidifier::Start() {
	if (!shouldChange)
		return;

	if (onChanged)
		onChanged(labyrinth.getWalls());
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

void LabyrinthSolidifier::OnWallDestroyedChanged(GameObject* wall) {
	if (wall->IsDestroyed()) {
		colliderMemory.Free(wall);
	}
	else {
		colliderMemory.Claim(wall);
	}
}