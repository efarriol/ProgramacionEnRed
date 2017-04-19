#pragma once
#include "System.h"
#include <math.h>
#include "Constants.h"
#include "Entities.h"

class Asteroid : public Entities{
public:
	Asteroid();
	~Asteroid();

	void Update(float deltaTime);
	void Setup();
	void ChangeSprite(ObjectID objectID, int newWidth, int newHight);
	std::string asteroidState;
	double speed;
	void RandomizeDirection();
	void InverseDirection();
	int id;
	Vector2D playerPosition;

private:
	Vector2D randomDirection;
	void UpdatePosition(float deltaTime);
};

