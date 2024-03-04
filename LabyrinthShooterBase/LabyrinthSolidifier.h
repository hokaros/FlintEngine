#pragma once
#include "../FlatEngine/GameObject.h"
#include "../FlatEngine/RectangleRenderer.h"
#include "../FlatEngine/Window.h"
#include "../LabyrinthShooterBase/ColliderMemory.h"
#include "../LabyrinthShooterBase/Regenerable.h"
#include "../LabyrinthShooterBase/Destroyable.h"
#include "../LabyrinthShooterBase/Labirynt.h"

#define WALL_REGEN 5.0

// Zamienia informacje o labiryncie na obiekty w grze i zarz¹dza nimi
class LabyrinthSolidifier 
	: public ObjectComponent
{
	DECLARE_COMPONENT();
public:
	LabyrinthSolidifier() = default;
	LabyrinthSolidifier(const Vector& pos,
		int wallWidth, int wallLength,
		int xCount, int yCount,
		double changeTime,
		bool shouldChange = false
	);
	~LabyrinthSolidifier();

	const Labirynt& GetLab() const;
	int WallsCount() const;
	GameObject** GetWalls() const;
	int BorderElements() const;
	GameObject** GetBorder() const;
	Vector GetSize() const;

	void SetLab(bool* walls);
	void ChangeLab();
	const ColliderMemory& GetColliderMemory() const;

	virtual void Awake() override;
	virtual void Update() override;

	virtual std::unique_ptr<ObjectComponent> Copy() override;

	static Vector LabyrinthSize(int wallWidth, int wallLength, int xCount, int yCount);

private:
	void PlaceWalls();

	GameObject* BuildWall(const Vector& size);
	GameObject* BuildWall(const Vector& size, const Rgb8& color);
	void BuildBorder();
	GameObject** BuildGateWall(Direction side);

	void OnWallDestroyedChanged(Destroyable& wall);

private:
	Vector position;
	DECLARE_FIELD(position);

	int wallWidth = 10;
	DECLARE_FIELD(wallWidth);

	int wallLength = 100;
	DECLARE_FIELD(wallLength);

	int xCount = 7;
	DECLARE_FIELD(xCount);

	int yCount = 5;
	DECLARE_FIELD(yCount);

	Rgb8 m_WallColor = Rgb8(0x00, 0x00, 0xAA);
	DECLARE_FIELD(m_WallColor);

	Rgb8 m_GateColor = Rgb8(0x00, 0xCC, 0xAA);
	DECLARE_FIELD(m_GateColor);

	bool shouldChange = true;
	DECLARE_FIELD(shouldChange);

	double changeTime = 2.0;
	DECLARE_FIELD(changeTime);

	double timeSinceLastChange = 0.0;
	DECLARE_FIELD(timeSinceLastChange);

	std::unique_ptr<Labirynt> labyrinth;
	std::unique_ptr<ColliderMemory> colliderMemory;

	GameObject** walls; // œciany podzielone na pionowe i poziome
	GameObject** border; // czêœci obwódki

	int borderCount;
};

