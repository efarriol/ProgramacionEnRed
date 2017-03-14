#include "Ship.h"



Ship::Ship(sf::Vector2i _position, ShipType _shipType, Faction faction, sf::Texture &texture)
{
	position = _position;
	isRotated = false;
	shipType = _shipType;
	sprite.setTexture(texture);
	isPlaced = false;
	switch (shipType)
	{
	default:
	case BIG:
		sprite.setTextureRect(sf::IntRect(0 + faction * 64,0,64 + faction * 64,256));
		break;
	case MEDIUM:
		sprite.setTextureRect(sf::IntRect(128 + faction * 64, 0, 64, 192));
		break;
	case SMALL:
		sprite.setTextureRect(sf::IntRect(256 + faction * 64, 0, 64, 128));
		break;
	}
	sprite.setPosition(position.x, position.y);
	sprite.setScale(1, 1);
}

Ship::Ship()
{
}


Ship::~Ship()
{
}

void Ship::Update()
{
	if(!isPlaced){
		sprite.setPosition(position.x, position.y);
	}
	if (isRotated) {
		sprite.setRotation(-90);
		sprite.setPosition(position.x, position.y+64);
	}
	else sprite.setRotation(0);
}

void Ship::SetPosition(int _x, int _y)
{
	position.x = _x; 
	position.y = _y;
}

void Ship::SetPlaced(bool _isPlaced)
{
	isPlaced = _isPlaced;
}

sf::Vector2i Ship::GetPosition()
{
	return position;
}

bool Ship::GetPlaced()
{
	return isPlaced;
}

ShipType Ship::GetType()
{
	return shipType;
}

bool Ship::GetRotation()
{
	return isRotated;
}

void Ship::SetRotation()
{
	if (isRotated) isRotated = false;
	else isRotated = true;
}

void Ship::Render(sf::RenderWindow &window)
{
	window.draw(sprite);
}
