#pragma once
#include "PlayersInfo.h"

struct MovementMessage {
	int id = 0;
	sf::Vector2i delta = sf::Vector2i(0,0);
	sf::Vector2i absolutePos = sf::Vector2i(0, 0);
	int angle = 0;
	MovementMessage(int _id, sf::Vector2i _delta, sf::Vector2i _absolutePos, int _angle) : id(_id), delta(_delta), absolutePos(_absolutePos), angle(_angle) {};
	MovementMessage() {};
};


class CriticalMessage
{
public:
	int playerId = 0;
	int messageId = 0; 
	int angle = 0;
	int movementMessageID = 0;
	int opponentId = 0;
	PlayerInfo::PacketType packetType = PlayerInfo::PacketType::PT_EMPTY;
	sf::Vector2i delta = sf::Vector2i(0,0);
	sf::Vector2i absolutePos = sf::Vector2i(0, 0);

	CriticalMessage(int _messageId, int _playerId, PlayerInfo::PacketType _packetType) : messageId(_messageId), playerId(_playerId), packetType(_packetType) {};
	CriticalMessage(int _messageId, int _playerId, PlayerInfo::PacketType _packetType, int _opponentId) : messageId(_messageId), playerId(_playerId), packetType(_packetType), opponentId(_opponentId) {};
	CriticalMessage(int _messageId, int _playerId, PlayerInfo::PacketType _packetType, int _movementMessageID, sf::Vector2i _delta, sf::Vector2i _absolutePos, int _angle) :messageId(_messageId), playerId(_playerId), packetType(_packetType), movementMessageID(_movementMessageID), delta(_delta), absolutePos(_absolutePos), angle(_angle){};
	~CriticalMessage();
};

