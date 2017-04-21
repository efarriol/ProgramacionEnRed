#pragma once
#include <SFML\Graphics.hpp>
#include <SFML\Network.hpp>
#include <iostream>

struct MovementInfo {
	int ID;
	sf::Vector2i delta;
	sf::Vector2i absolutePos;
};


class PlayerInfo
{
public:

	enum PacketType
	{
		PT_ACK,
		PT_PING,
		PT_EMPTY,
		PT_HELLO,
		PT_WELCOME,
		PT_MOVEMENT,
		PT_GAMESTART,
		PT_DISCONNECT
	};


	std::string name;
	int score;
	sf::IpAddress ipAdress;
	sf::Vector2i position;
	unsigned short port;
	int id;
	sf::CircleShape dot;
	sf::Time pingTime;
	sf::Clock pingClock;
	sf::Clock timeoutClock;
	sf::Time timeoutTime;
	bool setupDone = false; 
	MovementInfo movementInfo;

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

	PlayerInfo(int _id, sf::Vector2i _position, sf::Color color) {
		dot.setFillColor(color);
		position = _position;
		dot.setPosition(position.x, position.y);
		dot.setRadius(32);
	}

	void PlayerInfo::Render(sf::RenderWindow &window) {
		window.draw(dot);
	}

	~PlayerInfo() {};
};