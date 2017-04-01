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
	sf::Vector2i position;
	unsigned short port;

	//Constructor
	PlayerInfo();
	PlayerInfo(std::string _name, sf::Vector2i _position, sf::IpAddress _ipAdress, unsigned short _port) {
		name = _name;
		position = _position;
		ipAdress = _ipAdress;
		port = _port;
	};

	~PlayerInfo();
};