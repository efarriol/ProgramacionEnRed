#pragma once
#include <SFML\Graphics.hpp>
#include <SFML\Network.hpp>
#include <string>
#include <cstring>
#include <iostream>
#include <fstream>
#include <vector>
#include <Windows.h>
#include <PlayerInfo.h>
#define MAX_PLAYERS 4

sf::Vector2i CreateRandomPosition(std::vector<PlayerInfo*> &playersList) {
	sf::Vector2i randomPos;
	bool validRandom = false;
	int randomCount = 0;
	randomPos.x = rand() % 9 * 64;
	randomPos.y = rand() % 9 * 64;
	while (!validRandom) {
		for (int i = 0; i < playersList.size(); i++) {
			if (playersList[i]->position == randomPos) {
				randomPos.x = rand() % 9 * 64;
				randomPos.y = rand() % 9 * 64;
				break;
			}
			else randomCount++;
		}
		if (randomCount == playersList.size()) validRandom = true;
	}
	return randomPos;
}

int main(){
	srand(time(NULL));
	sf::IpAddress ip = sf::IpAddress::getLocalAddress();
	sf::IpAddress senderIP;
	unsigned short senderPort;
	sf::UdpSocket socket;
	sf::Packet packet;
	socket.setBlocking(false);
	std::vector<PlayerInfo*> playersList;
	int playersCount = 0;

	sf::Socket::Status status = socket.bind(5001);
	if (status != sf::Socket::Done) {
		std::cout << "Can't bind to port 5000" << std::endl;
	}

	std::string message;
	std::string playerName;

	//Start main loop
	while (true) {
		sf::sleep(sf::milliseconds(50));
		packet.clear();
		socket.receive(packet, senderIP, senderPort);
		packet >> message;
		std::cout << playersCount << std::endl;
		if (message == "Hello") {
			packet >> playerName;
			sf::Vector2i randomPos = CreateRandomPosition(playersList);
			packet.clear();

			bool exists = false;
			for (int i = 0; i < playersList.size(); i++) {
				if (playersList[i]->ipAdress == senderIP && playersList[i]->port == senderPort) {
					exists = true;
					break;
				}
			}
			if (!exists) {
				//Create new player
				playersList.push_back(new PlayerInfo(playerName, randomPos, senderIP, senderPort));
				//Send all players position to user
				packet << "Welcome" << playersList[playersCount]->position.x << playersList[playersCount]->position.y << playersCount;
				for (int i = 0; i < playersCount; i++) {
					packet << playersList[i]->position.x << playersList[i]->position.y;
				}
				socket.send(packet, playersList[playersCount]->ipAdress, playersList[playersCount]->port);
				packet.clear();
				//Send new player position to other users
				packet << "NewPlayer" << playersList[playersCount]->position.x << playersList[playersCount]->position.y;
				for (int i = 0; i < playersList.size(); i++) {
					if (i != playersCount) socket.send(packet, playersList[i]->ipAdress, playersList[i]->port);
				}
				playersCount++;
			}
		}
	}
}


