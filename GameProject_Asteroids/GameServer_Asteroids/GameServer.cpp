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
#include <PlayersInfo.h>
#include "CriticalMessage.h"


int main() {
	srand(time(NULL));
	sf::IpAddress ip = sf::IpAddress::getLocalAddress();
	sf::IpAddress senderIP;
	std::vector<CriticalMessage*> criticalMessages;	
	int criticalCount = 0;
	//PlayerInfo
	PlayerInfo player[2];
	bool setupDone = false;

	int playersCount = 0;
	unsigned short senderPort;
	sf::UdpSocket socket;

	socket.setBlocking(false);
	sf::Socket::Status status = socket.bind(5001);
	if (status != sf::Socket::Done) {
		std::cout << "Can't bind to port 5000" << std::endl;
	}
	while (true) {
		PlayerInfo::PacketType packetType;
		packetType = PlayerInfo::PacketType::PT_EMPTY;
		sf::sleep(sf::milliseconds(50));
		char messageBuffer[2000];
		int playerID = 0;
		size_t messageSize = 0;
		socket.receive(messageBuffer, sizeof(messageBuffer), messageSize, senderIP, senderPort);
		std::string stringMessage;
		InputMemoryBitStream imbs(messageBuffer, messageSize * 8);
		if (messageSize > 0) {
			imbs.Read(&playerID, 1);
			imbs.Read(&packetType, 3);
		}
		int messageId = 0;
		OutputMemoryBitStream ombs;
		switch (packetType)
		{
		case PlayerInfo::PT_ACK:
			imbs.Read(&messageId, 5);
			if(criticalMessages.size() > 0)	criticalMessages.erase(criticalMessages.begin() + messageId);
			break;
		case PlayerInfo::PT_PING:
			break;
		case PlayerInfo::PT_HELLO:
			if (playersCount < 2 && senderPort != player[0].port && senderPort != player[1].port) {
				player[playersCount].id = playersCount;
				player[playersCount].ipAdress = senderIP;
				player[playersCount].port = senderPort;
				if (senderPort == player[0].port) {
					ombs.Write(player[0].id, 1);
					ombs.Write(PlayerInfo::PacketType::PT_WELCOME, 3);
					socket.send(ombs.GetBufferPtr(), ombs.GetByteLength(), senderIP, senderPort);
				}
				else if (senderPort == player[1].port && !setupDone) {
					criticalMessages.push_back(new CriticalMessage(player[0].id, PlayerInfo::PacketType::PT_GAMESTART));
					criticalCount++;
					criticalMessages.push_back(new CriticalMessage(player[1].id, PlayerInfo::PacketType::PT_GAMESTART));
					criticalCount++;
					setupDone = true;
				}
				playersCount++;
			}
			break;
		case PlayerInfo::PT_MOVEMENT:
			break;
		case PlayerInfo::PT_DISCONNECT:
			break;
		default:
			break;
		}
		std::cout << criticalMessages.size() << std::endl;

		//Send criticalMessages
		for (int i = 0; i < criticalMessages.size(); i++) {
			OutputMemoryBitStream ombs2;
			ombs2.Write(criticalMessages[i]->id, 1);
			ombs2.Write(criticalMessages[i]->packetType, 3);
			switch (criticalMessages[i]->packetType) {
			case PlayerInfo::PacketType::PT_MOVEMENT:
					break;
			}
			ombs2.Write(i, 5);
			socket.send(ombs2.GetBufferPtr(), ombs2.GetByteLength(), player[criticalMessages[i]->id].ipAdress, player[criticalMessages[i]->id].port);
		}
	}
}