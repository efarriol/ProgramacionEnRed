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


int main() {
	srand(time(NULL));
	sf::IpAddress ip = sf::IpAddress::getLocalAddress();
	sf::IpAddress senderIP;
	//PlayerInfo
	PlayerInfo player[2];

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
			imbs.Read(&packetType, 3);
			imbs.Read(&playerID, 1);
			
		}
		if (packetType == PlayerInfo::PacketType::PT_HELLO) {
			if (playersCount < 2 && senderPort != player[0].port && senderPort != player[1].port) {
				player[playersCount].id = playersCount;
				player[playersCount].port = senderPort;			
				playersCount++;
			}
			OutputMemoryBitStream ombs;
			if (senderPort == player[0].port) {
				ombs.Write(PlayerInfo::PacketType::PT_WELCOME, 3);
				ombs.Write(player[0].id, 1);
				socket.send(ombs.GetBufferPtr(), ombs.GetByteLength(), senderIP, senderPort);
			}
			else if (senderPort == player[1].port) {
				ombs.Write(PlayerInfo::PacketType::PT_WELCOME, 3);
				ombs.Write(player[1].id, 1);
				socket.send(ombs.GetBufferPtr(), ombs.GetByteLength(), senderIP, senderPort);
			}
		}

		else if (packetType == PlayerInfo::PacketType::PT_MOVEMENT) {


		}
		if (playersCount == 2) {
			OutputMemoryBitStream ombs;
			ombs.Write(2);
			socket.send(ombs.GetBufferPtr(), ombs.GetByteLength(), senderIP, player1Port);
			socket.send(ombs.GetBufferPtr(), ombs.GetByteLength(), senderIP, player2Port);
		}

	}
}