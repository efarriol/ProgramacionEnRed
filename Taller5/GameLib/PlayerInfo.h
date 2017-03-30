#pragma once
#include <SFML\Graphics.hpp>
#include <SFML\Network.hpp>
#include <iostream>


class PlayerInfo
{
public:

	std::string name;
	int score;
	sf::IpAddress ipAdress;
	unsigned short port;

	//Constructor
	PlayerInfo() {
	};

	~PlayerInfo();
};