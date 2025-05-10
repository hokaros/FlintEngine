#pragma once

class GameBase;
class UnitTestKeyCreator;

class SystemCreationKey
{
private:
	SystemCreationKey() = default;

	friend GameBase;
	friend UnitTestKeyCreator;
};