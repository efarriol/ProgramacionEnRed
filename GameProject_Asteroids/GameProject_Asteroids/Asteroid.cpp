#include "Asteroid.h"



Asteroid::Asteroid()
{
	playerPosition = Vector2D(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
	Setup();
}

void Asteroid::Setup() {

	//Randomize a far position from player
	int randCorner = rand() % 4 + 1;
	switch (randCorner)
	{
	case 1:
		position = Vector2D(rand() % SCREEN_WIDTH, rand() % int(playerPosition.y-150)); //Random position inside up zone
		break;
	case 2:
		position = Vector2D(rand() % int(playerPosition.x - 200), rand() % SCREEN_HEIGHT); //Random position inside left zone
		break;
	case 3:
		position = Vector2D(rand() % SCREEN_WIDTH, rand() % (SCREEN_HEIGHT - int(playerPosition.y + 150)) + int(playerPosition.y + 150)); //Random position inside bottom zone
		break;
	case 4:
		position = Vector2D(rand() % (SCREEN_WIDTH - int(playerPosition.x + 150)) + int(playerPosition.x + 150), rand() % SCREEN_HEIGHT); //Random position right zone
		break;
	}

	RandomizeDirection();
	width = 92;
	height = 92;
	asteroidState = "BIG";
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


void Asteroid::RandomizeDirection()
{
	randomDirection = Vector2D(rand() % SCREEN_WIDTH, rand() % SCREEN_HEIGHT) - position; //Random Vector
	randomDirection.Normalize();
}

void Asteroid::InverseDirection() {
	randomDirection.x *= -1;
	randomDirection.y *= -1;
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
