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
	unsigned short player1Port = NULL;
	unsigned short player2Port = NULL;


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
		size_t messageSize = 0;
		socket.receive(messageBuffer, sizeof(messageBuffer), messageSize, senderIP, senderPort);
		std::string stringMessage;
		InputMemoryBitStream imbs(messageBuffer, messageSize * 8);
		if (messageSize > 0) {
			imbs.Read(&packetType);
			imbs.ReadString(&stringMessage);
			std::cout << stringMessage << std::endl;
		}
		if (packetType == PlayerInfo::PacketType::PT_HELLO  && senderPort != player1Port && senderPort != player2Port) {
			//creasPlayer
			if (playersCount == 0) player1Port = senderPort;
			else if (playersCount == 1) player2Port = senderPort;
			OutputMemoryBitStream ombs;
			ombs.Write(1);
			socket.send(ombs.GetBufferPtr(), ombs.GetByteLength(), senderIP, senderPort);
			playersCount++;
		}
		if (playersCount == 2) {
			OutputMemoryBitStream ombs;
			ombs.Write(2);
			socket.send(ombs.GetBufferPtr(), ombs.GetByteLength(), senderIP, player1Port);
			socket.send(ombs.GetBufferPtr(), ombs.GetByteLength(), senderIP, player2Port);
		}

	}
}