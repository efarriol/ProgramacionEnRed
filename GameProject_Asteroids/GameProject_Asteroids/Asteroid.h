#pragma once
#include "System.h"
#include <math.h>
#include "Constants.h"
#include "Entities.h"

class Asteroid : public Entities{
public:
	Asteroid(Vector2D _position, Vector2D _randomDirection, int _speed);
	~Asteroid();

	void Update(float deltaTime);
	void Setup(Vector2D _position, Vector2D _randomDirection, int _speed);
	void ChangeSprite(ObjectID objectID, int newWidth, int newHight);
	std::string asteroidState;
	float speed;
	int id;

private:
	Vector2D randomDirection;
	void UpdatePosition(float deltaTime);
};

