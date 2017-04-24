#pragma once
#include "PlayersInfo.h"
class CriticalMessage
{
public:
	int id;
	PlayerInfo::PacketType packetType;
	sf::Vector2i position;
	std::string message;
	CriticalMessage(int _id, PlayerInfo::PacketType _packetType) : id(_id), packetType(_packetType) {};
	CriticalMessage(int _id, PlayerInfo::PacketType _packetType, sf::Vector2i _position) :id(_id), packetType(_packetType), position(_position) {};
	CriticalMessage( int _id, PlayerInfo::PacketType _packetType, std::string _message) :id(_id), packetType(_packetType), message(_message) {};
	~CriticalMessage();
};

