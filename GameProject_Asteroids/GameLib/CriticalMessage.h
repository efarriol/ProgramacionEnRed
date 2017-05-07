#pragma once
#include "PlayersInfo.h"

struct MovementMessage {
	int id = 0;
	sf::Vector2i delta = sf::Vector2i(0,0);
	sf::Vector2i absolutePos = sf::Vector2i(0, 0);
	int angle = 0;
	MovementMessage(int _id, sf::Vector2i _delta, sf::Vector2i _absolutePos, int _angle) : id(_id), delta(_delta), absolutePos(_absolutePos), angle(_angle) {};
};


class CriticalMessage
{
public:
	int id;
	int opponentId;
	PlayerInfo::PacketType packetType;
	sf::Vector2i position;
	std::string message;
	CriticalMessage(int _id, PlayerInfo::PacketType _packetType) : id(_id), packetType(_packetType) {};
	CriticalMessage(int _id, PlayerInfo::PacketType _packetType, int _opponentId) : id(_id), packetType(_packetType), opponentId(_opponentId) {};
	CriticalMessage(int _id, PlayerInfo::PacketType _packetType, sf::Vector2i _position) :id(_id), packetType(_packetType), position(_position) {};
	CriticalMessage( int _id, PlayerInfo::PacketType _packetType, std::string _message) :id(_id), packetType(_packetType), message(_message) {};
	~CriticalMessage();
};

