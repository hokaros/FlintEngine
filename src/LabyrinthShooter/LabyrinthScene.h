#pragma once
#include <Scene.h>
#include <LabyrinthSolidifier.h>

class LabyrinthScene 
    : public Scene
{
public:
	LabyrinthScene();

private:
	void LoadFromFile(const char* file_path);
};

