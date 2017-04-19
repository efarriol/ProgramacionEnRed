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
		sf::sleep(sf::milliseconds(50));
		char messageBuffer[2000];
		size_t messageSize = 0;
		socket.receive(messageBuffer, sizeof(messageBuffer), messageSize, senderIP, senderPort);
		int message;
		InputMemoryBitStream imbs(messageBuffer, messageSize * 8);
		imbs.Read(&message);
		std::cout << senderPort << std::endl;

		if (message == 3 && senderPort != player1Port && senderPort != player2Port) {
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