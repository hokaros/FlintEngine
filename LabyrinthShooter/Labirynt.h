#pragma once
#include "Vector.h"

class Labirynt
{
	int x;
	int y;
	int walls_num;
	bool* walls;
	int* wall_buffer;
		// 2 * x * y - x - y	- scian
		// w rzedzie x-1 scian
		// w kolumnie y-1 scian
		// (x - 1)y scian poziomych
		// (y - 1)x scian pionowych

	void ShuffleWalls();
	void WallToCells(int wall, int* a, int* b);
	int CellsToWall(int indexA, int indexB);
	int CheckConnected(int indexA, int indexB, int* path);
	int CheckIter(int indexA, int indexB, bool* connected, int* path, int step);

public:
	Labirynt(int x, int y);

	void PrintLab();
	void ChangeLab();
	void SetWalls(bool* walls);
	bool* getWalls();
	int ActiveCount() const;
	static int MemorySize(int x, int y);
	int VerticalCount() const;
	int HorizontalCount() const;

	VectorInt GetExit(Direction side) const;

	bool operator[](int index);

	~Labirynt();
};

