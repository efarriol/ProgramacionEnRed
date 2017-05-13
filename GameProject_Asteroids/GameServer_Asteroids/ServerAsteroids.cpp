#include "ServerAsteroids.h"



ServerAsteroids::ServerAsteroids(int _id)
{
	id = _id;
	Setup();
}


ServerAsteroids::~ServerAsteroids()
{
}

void ServerAsteroids::Setup(){
	position = sf::Vector2i(rand() % SCREEN_WIDTH, rand() % SCREEN_HEIGHT);
	speed = 5;
	RandomizeDirection();
}

void ServerAsteroids::RandomizeDirection(){
	randomDirection = sf::Vector2i(rand() % SCREEN_WIDTH, rand() % SCREEN_HEIGHT) - position; //Random Vector
}
