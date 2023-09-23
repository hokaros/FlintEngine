#pragma once
#include "Labirynt.h"
#include "GameObject.h"
#include "RectangleRenderer.h"
#include "Regenerable.h"
#include "Window.h"
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
		const std::list<GameObject*>& allObjects,
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

	IUpdateable* Copy() override;
	static Vector LabyrinthSize(int wallWidth, int wallLength, int xCount, int yCount);
private:
	Vector position;

	int wallWidth;
	int wallLength;
	int xCount;
	int yCount;

	Labirynt labyrinth;
	ColliderMemory colliderMemory;

	const std::list<GameObject*>& allObjects;
	GameObject** walls; // œciany podzielone na pionowe i poziome
	GameObject** border; // czêœci obwódki
	int borderCount;

	int wallColor;
	int gateColor;

	bool shouldChange;
	double changeTime;
	double timeSinceLastChange;
private:
	void PlaceWalls();

	GameObject* BuildWall(const Vector& size);
	GameObject* BuildWall(const Vector& size, int color);
	void BuildBorder();
	GameObject** BuildGateWall(Direction side);

	void OnWallDestroyedChanged(GameObject* wall);
};

