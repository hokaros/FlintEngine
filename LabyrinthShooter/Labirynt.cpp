#include <iostream>
#include <cstdlib>
#include <time.h>
#include "Labirynt.h"

using namespace std;

Labirynt::Labirynt(int x, int y)
{
	this->x = x;
	this->y = y;
	walls_num = 2 * x * y - x - y;
	walls = (bool*)malloc(walls_num * sizeof(bool));
	wall_buffer = (int*)malloc(walls_num * sizeof(int));
	if (walls == NULL || wall_buffer == NULL) exit(1);

	srand(time(NULL));

	for (int i = 0; i < walls_num; i++)
	{
		wall_buffer[i] = i;
		walls[i] = true;
	}

	ShuffleWalls();

	int* path = (int*)malloc(walls_num * sizeof(int));
	if (path == NULL) exit(1);

	int counter = 0;
	for (int i = 0; i < walls_num; i++)
	{
		int a, b;
		WallToCells(wall_buffer[i], &a, &b);
		int steps = CheckConnected(a, b, path);
		if (steps < 0)
		{
			walls[wall_buffer[i]] = false;
			counter++;
		}
		if (counter == x * y - 1) break;
	}

	free(path);
}

void Labirynt::ShuffleWalls()
{
	int a, buf;
	for (int i = 0; i < walls_num; i++)
	{
		a = rand() % walls_num;
		buf = wall_buffer[i];
		wall_buffer[i] = wall_buffer[a];
		wall_buffer[a] = buf;
	}
}

void Labirynt::WallToCells(int wall, int* a, int* b)
{
	if (wall >= walls_num || wall < 0) { *a = 0; *b = 0; }
	else if (wall < (x - 1) * y)	// Poziome laczenia
	{
		int xc = wall % (this->x - 1);
		int yc = (wall - xc) / (this->x - 1);
		int index = xc + yc * this->x;
		*a = index;
		*b = index + 1;
	}
	else							// Pionowe laczenia
	{
		int wall_in = wall - (x - 1) * y;
		*a = wall_in;
		*b = wall_in + this->x;
	}
}

int Labirynt::CellsToWall(int indexA, int indexB)
{
	if (indexA < 0 || indexB < 0 || indexA >= 2 * x * y - x - y || indexB >= 2 * x * y - x - y) return -1;

	if (indexA - indexB == 1)
	{
		int xw = indexB % x;
		int yw = (indexB - xw) / x;
		int w = xw + yw * (x - 1);
		return w;
	}
	else if (indexA - indexB == -1)
	{
		int xw = indexA % x;
		int yw = (indexA - xw) / x;
		int w = xw + yw * (x - 1);
		return w;
	}
	else if (indexA - indexB == x)
	{
		int w = indexB + (x - 1) * y;
		return w;
	}
	else if (indexB - indexA == x)
	{
		int w = indexA + (x - 1) * y;
		return w;
	}
	else
	{
		return -1;
	}
}

int Labirynt::CheckConnected(int indexA, int indexB, int* path)
{
	bool* connected = (bool*)malloc(x * y * sizeof(bool));
	if (connected == NULL) exit(2);

	for (int i = 0; i < x * y; i++) connected[i] = false;

	int result = CheckIter(indexA, indexB, connected, path, 0);

	free(connected);



	return result;
}

int Labirynt::CheckIter(int indexA, int indexB, bool* connected, int* path, int step)
{
	if (indexA == indexB) return step;

	connected[indexA] = true;


	int w = CellsToWall(indexA, indexA + 1);
	if ((indexA + 1) % x == 0) w = -1;
	if (w != -1 && connected[indexA + 1] == false && walls[w] == false)
	{
		int val = CheckIter(indexA + 1, indexB, connected, path, step + 1);
		if (val >= 0) { path[step] = w; return val; }
	}

	w = CellsToWall(indexA, indexA - 1);
	if (indexA % x == 0) w = -1;
	if (w != -1 && connected[indexA - 1] == false && walls[w] == false)
	{
		int val = CheckIter(indexA - 1, indexB, connected, path, step + 1);
		if (val >= 0) { path[step] = w; return val; }
	}

	w = CellsToWall(indexA, indexA + x);
	if (indexA + x >= x * y) w = -1;
	if (w != -1 && connected[indexA + x] == false && walls[w] == false)
	{
		int val = CheckIter(indexA + x, indexB, connected, path, step + 1);
		if (val >= 0) { path[step] = w; return val; }
	}

	w = CellsToWall(indexA, indexA - x);
	if (w != -1 && connected[indexA - x] == false && walls[w] == false)
	{
		int val = CheckIter(indexA - x, indexB, connected, path, step + 1);
		if (val >= 0) { path[step] = w; return val; }
	}


	return -1;
}

void Labirynt::PrintLab()
{
	int* path = (int*)malloc(walls_num * sizeof(int));
	if (path == NULL) exit(1);
	int length = CheckConnected((y / 2) * x, (y / 2) * x + x - 1, path);
	int* cells = (int*)malloc((length + 1) * sizeof(int));
	if (cells == NULL) exit(1);
	for (int i = 0; i < length + 1; i++) cells[i] = -1;

	int index = 0;
	for (int i = 0; i < length; i++)
	{
		int A, B;
		bool a = true, b = true;
		WallToCells(path[i], &A, &B);
		for (int j = 0; j < i + 1; j++)
		{
			if (cells[j] == A) a = false;
			if (cells[j] == B) b = false;
		}
		if (a) { cells[index] = A; index++; }
		if (b) { cells[index] = B; index++; }
	}

	cout << "X: " << x << " Y: " << y << " Walls: " << walls_num << endl;
	for (int i = 0; i < y; i++)
	{
		cout << ' ';
		for (int j = 0; j < x; j++)
		{
			int index = j + i * x;
			bool up = false, down = false, left = false, right = false;
			if (j == 0) left = false;
			else if (walls[CellsToWall(index, index - 1)] == false) left = true;

			if (j == x - 1) right = false;
			else if (walls[CellsToWall(index, index + 1)] == false) right = true;

			if (i == 0) up = false;
			else if (walls[CellsToWall(index, index - x)] == false) up = true;

			if (i == y - 1) down = false;
			else if (walls[CellsToWall(index, index + x)] == false) down = true;

			if (j == 0 && i == y / 2) left = true;
			if (j == x - 1 && i == y / 2) right = true;

			/////////////////////////
			bool found = false;
			for (int k = 0; k < length + 1; k++)
			{
				if (cells[k] == j + i * x)
				{
					cout << 'X';
					found = true;
				}
			}
			if (found) continue;
			//////////////////////////

			if (up && down && left && right) cout << (char)206;
			else if (!up && down && left && right) cout << (char)203;
			else if (up && !down && left && right) cout << (char)202;
			else if (up && down && !left && right) cout << (char)204;
			else if (up && down && left && !right) cout << (char)185;
			else if (!up && !down && left && right) cout << (char)205;
			else if (!up && down && !left && right) cout << (char)201;
			else if (!up && down && left && !right) cout << (char)187;
			else if (up && !down && !left && right) cout << (char)200;
			else if (up && !down && left && !right) cout << (char)188;
			else if (up && down && !left && !right) cout << (char)186;
			else  cout << ' ';// (char)219;
		}
		cout << endl;
	}

	free(path);
	free(cells);

	// ╣ ║ ╗ ╝ ╚ ╩ ╔ ╦ ╠ ═ ╬ █
}

void Labirynt::SetWalls(bool* walls) {
	if (this->walls != NULL)
		delete[] this->walls;

	this->walls = walls;
}

void Labirynt::ChangeLab()
{
	ShuffleWalls();
	int wall = 0;
	int A, B;

	for (int i = 0; i < walls_num; i++)
	{
		if (walls[wall_buffer[i]])
		{
			wall = wall_buffer[i];
			break;
		}
	}
	WallToCells(wall, &A, &B);

	int* path = (int*)malloc(walls_num * sizeof(int));
	if (path == NULL) exit(1);
	int length = CheckConnected(A, B, path);

	int index = rand() % length;

	walls[path[index]] = true;
	walls[wall] = false;

	free(path);
}

bool* Labirynt::getWalls()
{
	return walls;
}

int Labirynt::ActiveCount() const {
	int counter = 0;
	for (int i = 0; i < walls_num; i++) {
		if (walls[i])
			counter++;
	}
	int theory = walls_num - x * y + 1;
	return counter; // TODO: czas stały
}

int Labirynt::MemorySize(int x, int y) {
	return 2 * x * y - x - y;
}

int Labirynt::VerticalCount() const {
	return (x - 1) * y;
}

int Labirynt::HorizontalCount() const {
	return (y - 1) * x;
}

VectorInt Labirynt::GetExit(Direction side) const {
	switch (side) {
	case Direction::EAST:
		return VectorInt(0, (y - 1) / 2);
	case Direction::WEST:
		return VectorInt(x, (y - 1) / 2);
	default:
		return GetExit(Direction::EAST);
	}
}

bool Labirynt::operator[](int index) {
	return walls[index];
}

Labirynt::~Labirynt()
{
	free(walls);
	free(wall_buffer);
}