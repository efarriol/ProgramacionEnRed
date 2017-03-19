#pragma once
#include <SFML\Graphics.hpp>
#include <SFML\Network.hpp>
#include <Windows.h>
#include "Vector4.h"

enum ShipType {
	SMALL, 
	MEDIUM, 
	BIG
 };
enum Faction {
	ISA,
	RSF
};

class Ship
{
private:
	sf::Sprite sprite;
	sf::Vector2i position;
	//Vector4i dimensions;
	bool isRotated;
	bool isPlaced;
	ShipType shipType;
	int damage;

public:
	Ship(sf::Vector2i _position, ShipType _shipType, Faction faction, sf::Texture &texture);
	Ship();
	~Ship();
	void Update();
	void SetPosition(int _x, int _y);
	void SetPlaced(bool _isPlaced);
	sf::Vector2i GetPosition();
	bool GetPlaced();
	ShipType GetType();
	sf::String GetBoatName(ShipType _shipType);
	bool GetRotation();
	void SetRotation();
	int GetDamage();
	void TakeDamage();
	void Render(sf::RenderWindow &window);
	void ChangeSprite(Faction faction);
};

