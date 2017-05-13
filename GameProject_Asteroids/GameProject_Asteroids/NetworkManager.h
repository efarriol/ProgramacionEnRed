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
#include "AsteroidsManager.h"

#define NM NetworkManager::Instance()
#define POSITIVE 1
#define NEGATIVE 0

class NetworkManager
{
private:
	sf::UdpSocket socket;
	sf::IpAddress serverIP = sf::IpAddress::getLocalAddress();
	sf::IpAddress senderIP;
	sf::Socket::Status status;
	unsigned short senderPort;
	Player *player;
	sf::Clock deltaClock;
	sf::Time deltaTime;
	bool firstClock = true;
	sf::Vector2i absolutePos = sf::Vector2i(0, 0);
	bool isWelcomed = false;
	std::vector <sf::Vector3i> absolutePositions; 
	int receivedAngle = 0;
	int receiveOpponentAngle = 0;
	float lambda = 0;
	float angleLambda;
public:
	inline static NetworkManager &Instance(void) {
		static NetworkManager networkManager;
		return networkManager;
	}

	NetworkManager();
	~NetworkManager();
	bool ConnectionEstablishment(Player* &player, OnlinePlayer* &onlinePlayer, AsteroidsManager* &asteroidsManager);
	void IngameConnection(Player* &player, OnlinePlayer* &onlinePlayer);
	void Disconnect();
};

