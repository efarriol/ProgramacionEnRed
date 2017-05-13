#include "Asteroid.h"



Asteroid::Asteroid(Vector2D _position, Vector2D _randomDirection, int _speed)
{
	Setup(_position, _randomDirection, _speed);
}

void Asteroid::Setup(Vector2D _position, Vector2D _randomDirection, int _speed) {
	width = 92;
	height = 92;
	asteroidState = "BIG";
	position = _position;
	randomDirection = _randomDirection;
	randomDirection.Normalize();
	speed = _speed/1000.0f;
	int randomSprite = rand() % (3) + 1;
	switch (randomSprite)
	{
	case 1:
		entitieSprite = { { int(position.x),int(position.y),width,height }, 0, ObjectID::BIG_ASTEROID1 };
		break;
	case 2:
		entitieSprite = { { int(position.x),int(position.y),width,height }, 0, ObjectID::BIG_ASTEROID2 };
		break;
	case 3:
		entitieSprite = { { int(position.x),int(position.y),width,height }, 0, ObjectID::BIG_ASTEROID3 };
	default:
		break;
	}
}

void Asteroid::ChangeSprite(ObjectID objectID, int newWidth, int newHight)
{
	width = newWidth;
	height = newHight;
	entitieSprite = { { int(position.x),int(position.y),width,height }, 0, objectID};
}

Asteroid::~Asteroid()
{
}

void Asteroid::Update(float deltaTime)
{
	DoWrap(position);
	UpdatePosition(deltaTime);
}


void Asteroid::UpdatePosition(float deltaTime)
{

	position.x += randomDirection.x*speed;
	position.y += randomDirection.y*speed;

	entitieSprite.transform.x = position.x;
	entitieSprite.transform.y = position.y;
	
}
