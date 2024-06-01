#pragma once
#include <Scene.h>
#include <AssetManager.h>
#include <LabyrinthSolidifier.h>

// Czas, co jaki zmienia siê labirynt
#define LAB_TIME 2

#define PLAYER_SIGHT 400

class LabyrinthScene 
    : public Scene
{
public:
	LabyrinthScene(const Vector& player_pos, AssetManager& asset_manager);

private:
	void LoadFromFile(const char* file_path);

	void LoadStartingObjects();

	virtual bool ShouldRender(GameObject* gameObject);
	GameObject* FindLabyrinth();
	GameObject* FindPlayer();
	GameObject* CreatePlayer(const Vector& position);

private:
	GameObject* m_Player;
	LabyrinthSolidifier* m_Lab;
	Vector m_PlayerStartingPos;

	AssetManager& m_AssetManager;
};

