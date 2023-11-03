#pragma once

class GameBase;

class SystemCreationKey
{
private:
	SystemCreationKey() = default;

	friend GameBase;
};