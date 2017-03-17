#pragma once
#include <SFML\Graphics.hpp>
#include "Fleet.h"
#include <iostream>

#define TIME 5
#define MAX_WORDS 10

class PlayerInfo
{
public:
	Fleet fleet;
	Grid &grid;
	int faction;
	std::string name;
	std::vector<std::string> messages;
	std::string input;
	int score;
	bool isReady;
	bool hasTurn;
	bool isImpact;
	sf::Vector2i shotCoords;
	std::vector<sf::Vector2i> coordRegister;


	PlayerInfo(std::string _name, Faction _faction, Grid &_grid) : grid(_grid), fleet(_faction, "./../Resources/Images/Spaceships.png", _grid) {
		name = _name;
		faction = _faction;
		score = 0;
	};

	~PlayerInfo();
};