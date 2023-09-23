#include "PlayerPositionsGenerator.h"
#include <ctime>

void PlayerPositionsGenerator::Generate(int positionCount, int mapWidth, int mapHeight, int mapX, int mapY)
{
	std::vector<float> positionsX;
	std::vector<float> positionsY;

	srand((unsigned int)time(NULL));
	const float offsetX = 50;
	const float offsetY = 50;

	for (int i = 0; i < positionCount; i++) {

		while (true) {

			bool invalidPosition = false;
			float tmpX = (float(std::rand()) / float((RAND_MAX)) * mapWidth) + mapX;
			float tmpY = (float(std::rand()) / float((RAND_MAX)) * mapHeight) + mapY;

			for (int j = 0; j < positionsX.size(); j++) {
				if (abs(positionsX[j] - tmpX) < offsetX && abs(positionsY[j] - tmpY) < offsetY) invalidPosition = true;
			}

			if (!invalidPosition) {
				positionsY.push_back(tmpY);
				positionsX.push_back(tmpX);
				break;
			}
		}
	}

	SavePlayerPositions(positionsX, positionsY);
}

Vector PlayerPositionsGenerator::GetPlayerPosition(int playerIndex) const
{
	return playerPositions[playerIndex];
}

const std::vector<Vector> PlayerPositionsGenerator::GetPlayerPositions() const
{
	return playerPositions;
}

void PlayerPositionsGenerator::SavePlayerPositions(const std::vector<float> xPositions, const std::vector<float> yPositions)
{
	int count = std::min(xPositions.size(), yPositions.size());

	for (int i = 0; i < count; i++)
	{
		Vector mergedPos = Vector(xPositions[i], yPositions[i]);
		playerPositions.push_back(mergedPos);
	}
}
