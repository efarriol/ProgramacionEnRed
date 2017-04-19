#pragma once
#include "Asteroid.h"
#include "Player.h"

class AsteroidsManager
{
public:
	AsteroidsManager(int numAsteroids, Player &_player, float asteroidsVelocity, float _incrementalSpeed, int _targetLevelAsteroids);
	~AsteroidsManager();
	void Update();
	void Draw();
	int GetLevel();

private:
	Asteroid* asteroidsPool;
	Player* player;

	int numAsteroids;
	float incrementalSpeed;
	float timeLapse;
	int destroyedAsteroids;
	int targetLevelAsteroids;
	int level;
	float initialSpeed;
	int numBigAsteroids;

	void CollisionController(Asteroid& currentAsteroid);
	void DivideAsteroid(Asteroid& currentAsteroid);
	void CreateAsteroidFromAsteroid(Asteroid& currentAsteroid);
	void CreateAsteroid();
	void DeleteAsteroid(Asteroid& currentAsteroid);
	void NextLevel();
};

