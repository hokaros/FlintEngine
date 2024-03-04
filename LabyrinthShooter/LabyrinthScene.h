#pragma once
#include "../FlatEngine/Scene.h"
#include "../FlatEngine/AssetManager.h"
#include "../LabyrinthShooterBase/StatRenderer.h"
#include "../LabyrinthShooterBase/LabyrinthSolidifier.h"

// Czas, co jaki zmienia siê labirynt
#define LAB_TIME 2

#define PLAYER_SIGHT 400

class LabyrinthScene 
    : public Scene
{
public:
	LabyrinthScene(const Vector& player_pos, AssetManager& asset_manager);

	virtual void Update() override;
	virtual void Render() override;

private:
	void LoadFromFile(const char* file_path);

	void LoadStartingObjects();

	virtual bool ShouldRender(GameObject* gameObject);
	GameObject* FindPlayer();
	GameObject* CreatePlayer(const Vector& position);

private:
	GameObject* m_Player;
	std::unique_ptr<LabyrinthSolidifier> m_Lab;
	Vector m_PlayerStartingPos;

	std::unique_ptr<BMPStats> m_HealthStats = nullptr;

	AssetManager& m_AssetManager;
};

