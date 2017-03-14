#pragma once
#include <Windows.h>
#include <SFML\Graphics.hpp>
#include <SFML\Network.hpp>
#include "Ship.h"
#include "Grid.h"
#include <iostream>

#define MAX_SHIPS 5

class Fleet
{
private:
	//sf::RenderWindow window;
	Grid &grid;
	Faction fleetFaction;
	std::vector<Ship> ships;
	int shipCount = 0;
	int shipType = 2;
	sf::Texture shipTexture;

public:
	Fleet(Faction _fleetFaction, sf::Texture &texture, Grid &_grid);
	void PlaceFleet(sf::RenderWindow &window, sf::Event &evento, sf::Mouse &mouseEvent);
	void Render(sf::RenderWindow &window);
	~Fleet();
};

