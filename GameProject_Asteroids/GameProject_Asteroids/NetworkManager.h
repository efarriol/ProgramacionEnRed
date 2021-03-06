#pragma once
#include "NetworkManager.h"
#include <SFML\Network.hpp>
#include <InputOutputMemory\InputMemoryBitStream.h>
#include <InputOutputMemory\InputMemoryStream.h>
#include <InputOutputMemory\OutputMemoryBitStream.h>
#include <InputOutputMemory\OutputMemoryStream.h>
#include <PlayersInfo.h>
#include "Player.h"
#include "OnlinePlayer.h"
#include <CriticalMessage.h>

#define NM NetworkManager::Instance()
#define POSITIVE 1
#define NEGATIVE 0

class NetworkManager
{
private:
	sf::UdpSocket socket;
	sf::IpAddress serverIP = sf::IpAddress::getLocalAddress();
	sf::IpAddress senderIP;
	unsigned short senderPort;
	//Player *player;
	sf::Clock deltaClock;
	sf::Time deltaTime;
	bool firstClock = true;
	std::vector <MovementMessage> movementMessages;
	int idCount = 0;
	sf::Vector2i absolutePos = sf::Vector2i(0, 0);
public:
	inline static NetworkManager &Instance(void) {
		static NetworkManager networkManager;
		return networkManager;
	}

	NetworkManager();
	~NetworkManager();
	bool ConnectionEstablishment(Player* &player, OnlinePlayer* &onlinePlayer);
	void IngameConnection(Player* &player, OnlinePlayer* &onlinePlayer);
	void SendMovementMessage(int messageId, Player* &player);
	void Disconnect();
};

