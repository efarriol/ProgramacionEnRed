#pragma once
#include "NetworkManager.h"
#include <SFML\Network.hpp>
#include <InputOutputMemory\InputMemoryBitStream.h>
#include <InputOutputMemory\InputMemoryStream.h>
#include <InputOutputMemory\OutputMemoryBitStream.h>
#include <InputOutputMemory\OutputMemoryStream.h>
#include <PlayersInfo.h>
#include "Player.h"

#define NM NetworkManager::Instance()

class NetworkManager
{
private:
	sf::UdpSocket socket;
	sf::IpAddress serverIP = sf::IpAddress::getLocalAddress();
	sf::IpAddress senderIP;
	unsigned short senderPort;
	Player *player;
	sf::Clock deltaClock;
	sf::Time deltaTime;
	bool firstClock = true;

public:
	inline static NetworkManager &Instance(void) {
		static NetworkManager networkManager;
		return networkManager;
	}

	NetworkManager();
	~NetworkManager();
	bool ConnectionEstablishment(Player* &player);
	void IngameConnection(Player* &player);
	void Disconnect();
};

