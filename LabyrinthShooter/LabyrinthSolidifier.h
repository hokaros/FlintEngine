#pragma once
#include "Labirynt.h"
#include "../FlatEngine/GameObject.h"
#include "../FlatEngine/RectangleRenderer.h"
#include "Regenerable.h"
#include "../FlatEngine/Window.h"
#include "ColliderMemory.h"

#define WALL_REGEN 5.0

// Zamienia informacje o labiryncie na obiekty w grze i zarz¹dza nimi
class LabyrinthSolidifier : public IUpdateable
{
public:
	// Po wymieszaniu labiryntu
	function<void(bool*)> onChanged;

public:
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

	void Start() override;
	void Update() override;

	static Vector LabyrinthSize(int wallWidth, int wallLength, int xCount, int yCount);
private:
	Vector position;

	int wallWidth;
	int wallLength;
	int xCount;
	int yCount;

	Labirynt labyrinth;
	ColliderMemory colliderMemory;

	GameObject** walls; // œciany podzielone na pionowe i poziome
	GameObject** border; // czêœci obwódki
	int borderCount;

	Rgb8 m_WallColor;
	Rgb8 m_GateColor;

	bool shouldChange;
	double changeTime;
	double timeSinceLastChange;
private:
	void PlaceWalls();

	GameObject* BuildWall(const Vector& size);
	GameObject* BuildWall(const Vector& size, const Rgb8& color);
	void BuildBorder();
	GameObject** BuildGateWall(Direction side);

	void OnWallDestroyedChanged(GameObject* wall);
};

