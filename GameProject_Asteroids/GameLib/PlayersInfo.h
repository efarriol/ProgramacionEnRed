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

	sf::Vector2f accumuluedMovement;
	float angle;
	std::string name;
	int score;
	sf::IpAddress ipAdress;
	sf::Vector2i position;
	unsigned short port;
	int id;
	sf::Time pingTime;
	sf::Clock pingClock;
	sf::Clock timeoutClock;
	sf::Time timeoutTime;
	bool setupDone = false; 
	MovementInfo movementInfo;

	//Constructor
	PlayerInfo() {	
	};

	~PlayerInfo() {};
};