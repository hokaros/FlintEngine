#pragma once
#include "Vector.h"
#include <vector>

class PlayerPositionsGenerator
{
public:
	void Generate(int positionCount, int mapWidth, int mapHeight, int mapX, int mapY);

	Vector GetPlayerPosition(int playerIndex) const;
	const std::vector<Vector> GetPlayerPositions() const;

private:
	std::vector<Vector> playerPositions;

private:
	void SavePlayerPositions(const std::vector<float> xPositions, const std::vector<float> yPositions);
};

