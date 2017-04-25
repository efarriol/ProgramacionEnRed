
#pragma once
#include "Scene.h"
#include "Button.h"
#include "Constants.h"
#include "Player.h"
#include "OnlinePlayer.h"
#include "AsteroidsManager.h"
#include "OvniManager.h"
#include <XML\rapidxml.hpp>
#include <sstream>

#define MAX_ASTEROIDS 5

// GameScene class with the main gameplay mode
class GameScene : public Scene {
public:
	explicit GameScene();
	~GameScene() override;
	void OnEntry(void) override;
	void OnExit(void) override;
	void Update(void) override;
	void Draw(void) override;

private:

	Button backButton;
	Button continueButton;
	Button exitButton;
	Button exitButton2;

	//Level Attributes
	int numAsteroids;
	double asteroidsVelocity;
	float incrementalSpeed;
	int targetLevelAsteroid;
	float ovniSpeed;
	float ovniSpawnTime;

	int playerLifes;

	void Setup();
	void ReadFromFile(std::string path);

	bool inGameMenu;

	Player* player;
	OnlinePlayer* onlinePlayer;

	AsteroidsManager* asteroidsManager;
	OvniManager* ovniManager;

	Sprite lifeCounter;
	enum State {
		PLAY,
		LOBBY
	};
	State currentState;
};