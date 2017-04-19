#pragma once
#include <SFML\Graphics.hpp>
#include <SFML\Network.hpp>
#include <InputOutputMemory\InputMemoryBitStream.h>
#include <InputOutputMemory\InputMemoryStream.h>
#include <InputOutputMemory\OutputMemoryBitStream.h>
#include <InputOutputMemory\OutputMemoryStream.h>
#include <string>
#include <cstring>
#include <iostream>
#include <fstream>
#include <vector>
#include <Windows.h>
#include <PlayerInfo.h>
#define MAX_PLAYERS 4
#define PING_TIME 5
#define GRID_SIZE 64
#define PING_TIMEOUT 20


sf::Vector2i CreateRandomPosition(std::vector<PlayerInfo*> &playersList) {
	sf::Vector2i randomPos;
	bool validRandom = false;
	int randomCount = 0;
	randomPos.x = rand() % 9 * 64;
	randomPos.y = rand() % 9 * 64;
	while (!validRandom) {
		randomCount = 0;
		for (int i = 0; i < playersList.size(); i++) {
			if (playersList[i]->position.x == randomPos.x && playersList[i]->position.y == randomPos.y) {
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

	
	socket.setBlocking(false);
	std::vector<PlayerInfo*> playersList;
	int playersCount = 0;
	
	sf::Socket::Status status = socket.bind(5001);
	if (status != sf::Socket::Done) {
		std::cout << "Can't bind to port 5000" << std::endl;
	}

	std::string playerName;
	//Start main loop
	while (true) {
		sf::sleep(sf::milliseconds(50));
		char messageBuffer[2000];
		size_t messageSize = 0;
		socket.receive(messageBuffer, sizeof(messageBuffer), messageSize, senderIP, senderPort);
		InputMemoryBitStream imbs(messageBuffer, messageSize * 8);
		OutputMemoryBitStream ombs;
		std::string tempIP;
		PlayerInfo::PacketType packetType;
		packetType = PlayerInfo::PacketType::PT_EMPTY;
		int playerID = 0;

		imbs.Read(&packetType, 3);
		imbs.Read(&playerID, 7);
		imbs.ReadString(&tempIP);
		senderIP = tempIP;
		imbs.Read(&senderPort, 16);
		

		
		if (packetType == PlayerInfo::PacketType::PT_HELLO) {
			sf::Vector2i randomPos = CreateRandomPosition(playersList);
			bool exists = false;
			for (int i = 0; i < playersList.size(); i++) {
				if (playersList[i]->ipAdress == senderIP && playersList[i]->port == senderPort) {
					exists = true;
					break;
				}
			playersList[playerID]->timeoutClock.restart();
			}
			if (!exists) {
				playersList.push_back(new PlayerInfo(playersCount, playerName, randomPos, senderIP, senderPort));
				sf::Vector2i gridPosition;
				ombs.Write(PlayerInfo::PacketType::PT_WELCOME, 3);
				ombs.Write(playersCount, 7);
				for (int i = 0; i <= playersCount; i++) {
					gridPosition.x = int(playersList[i]->position.x / GRID_SIZE);
					gridPosition.y = int(playersList[i]->position.y / GRID_SIZE);

					ombs.Write(gridPosition.x, 4);
					ombs.Write(gridPosition.y, 4);
					ombs.Write(playersList[i]->id, 7);
				}
				socket.send(ombs.GetBufferPtr(), ombs.GetByteLength(), playersList[playersCount]->ipAdress, playersList[playersCount]->port);
				//ombs.~OutputMemoryBitStream();
				ombs.Write(PlayerInfo::PacketType::PT_NEWPLAYER, 3);
				ombs.Write(gridPosition.x, 4);
				ombs.Write(gridPosition.y, 4);
				ombs.Write(playersList[playersCount]->id, 7);
				for (int i = 0; i < playersList.size(); i++) {
					if (i != playersCount) socket.send(ombs.GetBufferPtr(), ombs.GetByteLength(), playersList[i]->ipAdress, playersList[i]->port);
				}
				playersCount++;
			}
		}
		else if (packetType == PlayerInfo::PacketType::PT_PING) {
			playersList[playerID]->timeoutClock.restart(); 
		}
		else if (packetType == PlayerInfo::PacketType::PT_DISCONNECT){
			delete playersList[playerID];
			playersList.erase(playersList.begin()+playerID);
			//ombs.~OutputMemoryBitStream();
			ombs.Write(PlayerInfo::PacketType::PT_DISCONNECT, 3);
			ombs.Write(playerID, 7);

			for (int i = 0; i < playersList.size(); i++) {
				if (i >= playerID) playersList[i]->id--;
				socket.send(ombs.GetBufferPtr(), ombs.GetByteLength(), playersList[i]->ipAdress, playersList[i]->port);
			} 
			playersCount--;
			std::cout << "Player " << playerID << " disconnected" << std::endl;
		}
		//MANAGE PING
		for (int i = 0; i < playersList.size(); i++) {
			playersList[i]->pingTime = playersList[i]->pingClock.getElapsedTime();
			playersList[i]->timeoutTime = playersList[i]->timeoutClock.getElapsedTime();
			if (playersList[i]->pingTime.asSeconds() > PING_TIME) {
				playersList[i]->pingClock.restart();
				//ombs.~OutputMemoryBitStream();
				ombs.Write(PlayerInfo::PacketType::PT_PING, 3);
				ombs.Write(playersList[i]->id,7);
				socket.send(ombs.GetBufferPtr(), ombs.GetByteLength(), playersList[i]->ipAdress, playersList[i]->port);
			}
			if (playersList[i]->timeoutTime.asSeconds() > PING_TIMEOUT) {
				delete playersList[i];
				playersList.erase(playersList.begin() + i);
				//ombs.~OutputMemoryBitStream();
				ombs.Write(PlayerInfo::PacketType::PT_DISCONNECT, 3);
				ombs.Write(i, 7);

				for (int j = 0; j < playersList.size(); j++) {
					if (j >= i) playersList[j]->id--;
					socket.send(ombs.GetBufferPtr(), ombs.GetByteLength(), playersList[j]->ipAdress, playersList[j]->port);
				}
				playersCount--;
				std::cout << "Player " << i << " disconnected" << std::endl;
			}
		}

	}
}


