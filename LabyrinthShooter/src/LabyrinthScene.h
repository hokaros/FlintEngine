#pragma once
#include <Scene.h>
#include <AssetManager.h>
#include <StatRenderer.h>
#include <LabyrinthSolidifier.h>

// Czas, co jaki zmienia siê labirynt
#define LAB_TIME 2

#define PLAYER_SIGHT 400

class LabyrinthScene 
    : public Scene
{
public:
	LabyrinthScene(const Vector& player_pos, AssetManager& asset_manager);

	virtual void Update() override;
	virtual void Render(SceneRenderer& renderer) override;

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

	std::unique_ptr<BMPStats> m_HealthStats = nullptr;

	AssetManager& m_AssetManager;
};

