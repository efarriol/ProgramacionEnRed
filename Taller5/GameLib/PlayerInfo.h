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
	int id;
	sf::CircleShape dot;

	//Constructor
	PlayerInfo() {
		dot.setFillColor(sf::Color(0, 255, 0, 150));
		dot.setPosition(position.x, position.y);
		dot.setRadius(32);
	};
	PlayerInfo(int _id, std::string _name, sf::Vector2i _position, sf::IpAddress _ipAdress, unsigned short _port) {
		id = _id;
		name = _name;
		position = _position;
		ipAdress = _ipAdress;
		port = _port;
		dot.setFillColor(sf::Color(0, 255, 0, 150));
		dot.setPosition(position.x, position.y);
		dot.setRadius(32);
	};

	PlayerInfo(int _id, sf::Vector2i _position ) {
		dot.setFillColor(sf::Color(255, 0, 0, 150));
		position = _position;
		dot.setPosition(position.x, position.y);	
		dot.setRadius(32);
	}

	void PlayerInfo::Render(sf::RenderWindow &window) {
		window.draw(dot);
	}

	~PlayerInfo() {};
};