#pragma once
#include <SFML\Graphics.hpp>
#include <SFML\Network.hpp>
#include <Windows.h>
#include "Vector4.h"
#define MAX_SHIPS 5
enum ShipType {
	BIG, 
	MEDIUM, 
	SMALL
 };
enum Faction {
	ISA,
	RSF
};

class Ship
{
private:
	sf::Sprite sprite;
	sf::Texture texture;
	sf::Vector2i position;
	//Vector4i dimensions;
	bool isRotated;
	bool isPlaced;

public:
	Ship(sf::Vector2i _position, ShipType shipType, Faction faction);
	Ship();
	~Ship();
	void Update();
	void SetPosition(int _x, int _y);
	void SetPlaced(bool _isPlaced);
	bool GetPlaced();
	void SetRotation();
	void Render(sf::RenderWindow &window);
};

