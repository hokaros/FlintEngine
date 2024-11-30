#include "LabyrinthSolidifier.h"
#include <Components/BoxCollider.h>
#include "Occlusion/OccludableRectangle.h"
#include <Core/Scene.h>

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
}

const Labirynt& LabyrinthSolidifier::GetLab() const
{
	return *labyrinth;
}

int LabyrinthSolidifier::WallsCount() const
{
	return labyrinth->ActiveCount();
}

GameObject** LabyrinthSolidifier::GetWalls() const
{
	return walls;
}

Vector LabyrinthSolidifier::GetSize() const
{
	return LabyrinthSize(wallWidth, wallLength, xCount, yCount);
}

Vector LabyrinthSolidifier::LabyrinthSize(int wallWidth, int wallLength, int xCount, int yCount)
{
	return Vector(wallLength * xCount, wallLength * yCount);
}

void LabyrinthSolidifier::PlaceWalls()
{
	int nextWall = 0;
	Vector verticalWall(wallWidth, wallLength);
	Vector horizontalWall(wallLength, wallWidth);

	// �ciany pionowe
	for (int y = 0; y < yCount; y++)
	{
		for (int x = 1; x <= xCount - 1; x++)
		{
			int index = (xCount - 1) * y + x - 1;
			if (labyrinth->WallAtIndex(index))
			{
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
	for (int y = 1; y <= yCount - 1; y++)
	{
		for (int x = 0; x < xCount; x++)
		{
			int index = horStart + xCount * (y - 1) + x;
			if (labyrinth->WallAtIndex(index))
			{
				// Obecna �ciana
				GameObject* wall = walls[nextWall++];
				wall->FindComponent<BoxCollider>()->SetSize(horizontalWall);
				wall->FindComponent<RectangleRenderer>()->SetSize(horizontalWall);
				SetWallUpperLeft(*wall, Vector(x * wallLength, y * wallLength) + position);
			}
		}
	}
}

void LabyrinthSolidifier::SetLab(bool* walls)
{
	labyrinth->SetWalls(walls);
	PlaceWalls();

	colliderMemory->Refresh(this->walls, labyrinth->ActiveCount());

	GetOwner().GetScene()->RegenerateNavmesh();
}

void LabyrinthSolidifier::ChangeLab()
{
	labyrinth->ChangeLab();
	PlaceWalls();

	colliderMemory->Refresh(walls, labyrinth->ActiveCount());

	GetOwner().GetScene()->RegenerateNavmesh();
}

const ColliderMemory& LabyrinthSolidifier::GetColliderMemory() const
{
	return *colliderMemory;
}

GameObject* LabyrinthSolidifier::BuildWall(const Vector& size)
{
	return BuildWall(size, m_WallColor);
}

GameObject* LabyrinthSolidifier::BuildWall(const Vector& size, const Rgb8& color)
{
	const GameObject* wallPrefabLoaded = wallPrefab.Get();
	FE_ASSERT(wallPrefabLoaded != nullptr, "Couldn't load wall prefab");

	GameObject* wall = GameObject::Instantiate(*wallPrefabLoaded);

	wall->FindComponent<BoxCollider>()->SetSize(size);

	wall->FindComponent<RectangleRenderer>()->SetColor(color);
	wall->FindComponent<RectangleRenderer>()->SetSize(size);

	return wall;
}

void LabyrinthSolidifier::BuildBorder()
{
	m_Border.clear();

	m_Border.resize(8);
	BuildGateWall(Direction::EAST, m_Border[0], m_Border[1], m_Border[2]);
	BuildGateWall(Direction::WEST, m_Border[3], m_Border[4], m_Border[5]);

	Vector nextPos = position;
	Vector elemSize(GetSize().x, wallWidth);
	m_Border[6] = BuildWall(elemSize);
	SetWallUpperLeft(*m_Border[6], nextPos);
	nextPos.y += yCount * wallLength;
	m_Border[7] = BuildWall(elemSize);
	SetWallUpperLeft(*m_Border[7], nextPos);
}

void LabyrinthSolidifier::BuildGateWall(Direction side, GameObject*& go1, GameObject*& go2, GameObject*& go3)
{
	Vector nextPos = position;
	if (side == Direction::WEST)
	{
		nextPos.x += xCount * wallLength;
	}
	else if (side == Direction::SOUTH)
	{
		nextPos.y += wallWidth + yCount * wallLength;
	}
	VectorInt exit = labyrinth->GetExit(side);
	
	if (side == Direction::EAST || side == Direction::WEST)
	{
		Vector elemSize(wallWidth, exit.y * wallLength);
		go1 = BuildWall(elemSize);
		SetWallUpperLeft(*go1, nextPos);
		nextPos.y += elemSize.y;

		// Brama
		elemSize = Vector(wallWidth, wallLength);
		go2 = BuildWall(elemSize, m_GateColor);
		SetWallUpperLeft(*go2, nextPos);
		nextPos.y += elemSize.y;

		elemSize = Vector(
			wallWidth,
			(yCount - exit.y - 1) * wallLength
		);
		go3 = BuildWall(elemSize);
		SetWallUpperLeft(*go3, nextPos);
	}
	else
	{
		Vector elemSize(exit.x * wallLength, wallWidth);
		go1 = BuildWall(elemSize);
		SetWallUpperLeft(*go1, nextPos);
		nextPos.x += elemSize.x;

		// Brama
		elemSize = Vector(wallLength, wallWidth);
		go2 = BuildWall(elemSize, m_GateColor);
		SetWallUpperLeft(*go2, nextPos);
		nextPos.x += elemSize.x;

		elemSize = Vector(
			(xCount - exit.x - 1) * wallLength,
			wallWidth
		);
		go3 = BuildWall(elemSize);
		SetWallUpperLeft(*go3, nextPos);
	}
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
	for (GameObject* border : m_Border)
	{
		OccludableRectangle* occludable = border->FindComponent<OccludableRectangle>();
		std::unique_ptr<RectangleRenderer> unoccludable = std::make_unique<RectangleRenderer>();
		unoccludable->SetColor(occludable->GetColor());
		unoccludable->SetSize(occludable->GetSize());

		border->AddComponent(std::move(unoccludable));
		border->RemoveComponent(occludable);
	}

	colliderMemory->Refresh(walls, labyrinth->ActiveCount());
	labyrinth->PrintLab(); // wy�wietlenie w konsoli

	GetOwner().GetScene()->RegenerateNavmesh();
}

void LabyrinthSolidifier::Update()
{
	if (!shouldChange)
		return;

	timeSinceLastChange += Timer::Main()->GetDeltaTime();
	if (timeSinceLastChange >= changeTime)
	{
		ChangeLab();
		timeSinceLastChange = 0;
	}
}

void LabyrinthSolidifier::RenderEditorSelected(SceneRenderer& renderer)
{
	const Vector labyrinth_size = LabyrinthSize(wallWidth, wallLength, xCount, yCount);
	const Vector pos = GetOwner().TransformPoint(position);

	renderer.RenderWireRect(Rect(pos, labyrinth_size), Rgb8(0x00, 0x00, 0xFF), 0);
}

void LabyrinthSolidifier::OnWallDestroyedChanged(Destroyable& wall)
{
	if (wall.IsDestroyed())
	{
		colliderMemory->Free(&wall.GetOwner());
	}
	else
	{
		colliderMemory->Claim(&wall.GetOwner());
	}
}