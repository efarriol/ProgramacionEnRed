#pragma once
#include <SFML\Network.hpp>
#define SCREEN_WIDTH 1024
#define SCREEN_HEIGHT 768
class ServerAsteroids
{
public:
	ServerAsteroids(int _id);
	~ServerAsteroids();

	void Setup();
	int id = 0;
	sf::Vector2i position = sf::Vector2i(0,0);
	sf::Vector2i randomDirection = sf::Vector2i(0,0);
	int speed = 0;
	void RandomizeDirection();

};

