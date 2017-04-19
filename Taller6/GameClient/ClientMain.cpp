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
#include <vector>
#include <Windows.h>
#include <PlayerInfo.h>

int main()
{
	//Init udp variables
	std::vector <PlayerInfo *> playersList;
	sf::IpAddress serverIP = sf::IpAddress::getLocalAddress();
	sf::IpAddress senderIP;
	unsigned short senderPort;
	sf::UdpSocket socket;

	char messageBuffer[2000];
	size_t messageSize = 0;
	
	OutputMemoryBitStream ombs;

	socket.setBlocking(false);
	std::string playerNick;
	//Init impact dots vector
	sf::Vector2i dotPosition;
	bool setUpDone = false;
	
	int playersOnline = 0;
	int receivedId = 0;
	int clientId = 0;
	//Init Textures

	sf::Texture blue_grid;
	blue_grid.loadFromFile("./../Resources/Images/blue_grid.png");
	sf::Sprite grid;
	grid.setTexture(blue_grid);
	grid.setPosition(sf::Vector2f(0,0));

	//Init font and text...
	sf::Font font;
	if (!font.loadFromFile("courbd.ttf"))
	{
		std::cout << "Can't load the font file" << std::endl;
	}

	//Request name
	//std::cout << "Introduce your name:" << std::endl;
	//std::cin >> playerNick;
	ombs.Write(PlayerInfo::PacketType::PT_HELLO, 3);
	ombs.Write(NULL, 7);
	ombs.WriteString(sf::IpAddress::getLocalAddress().toString());
	ombs.Write(socket.getLocalPort(), 16);
	//ombs.WriteString(playerNick);
	//Init Windows
	sf::Vector2i screenDimensions(640, 640);
	sf::RenderWindow window;
	window.create(sf::VideoMode(screenDimensions.x, screenDimensions.y), "Taller 05");

	//Init events
	sf::Event evento;
	sf::Mouse mouseEvent;
	std::string receiveMessage;

	//Start GameLoop
	while (true) {
		window.pollEvent(evento);

		socket.receive(messageBuffer, sizeof(messageBuffer), messageSize, senderIP, senderPort);

		InputMemoryBitStream imbs(messageBuffer, messageSize * 8);
		PlayerInfo::PacketType packetType;
		packetType = PlayerInfo::PacketType::PT_EMPTY;
		OutputMemoryBitStream ombs;
		
		imbs.Read(&packetType,3);

		sf::sleep(sf::milliseconds(100));
		if (evento.key.code == sf::Keyboard::Escape) break;

		if (packetType != PlayerInfo::PacketType::PT_WELCOME && !setUpDone) {
			socket.send(ombs.GetBufferPtr(), ombs.GetByteLength(), serverIP, 5001);
		}
		else if (packetType == PlayerInfo::PacketType::PT_WELCOME) {
			if (!setUpDone) {
				int playersCount = 0;
				imbs.Read(&playersCount, 7);
				for (int i = 0; i < playersCount; i++) {
					imbs.Read(&dotPosition.x, 4);
					imbs.Read(&dotPosition.y, 4);
					imbs.Read(&receivedId, 7);
					playersList.push_back(new PlayerInfo(receivedId, sf::Vector2i(dotPosition.x * 64, dotPosition.y * 64), sf::Color(255, 0, 0, 150)));
				}
				imbs.Read(&dotPosition.x, 4);
				imbs.Read(&dotPosition.y, 4);
				imbs.Read(&receivedId, 7);
				playersList.push_back(new PlayerInfo(receivedId, dotPosition, sf::Color(0, 255, 0, 150)));
				clientId = receivedId;
			}
			//SEND
			//ombs.~OutputMemoryBitStream();
			ombs.Write(PlayerInfo::PacketType::PT_ACK, 3);
			ombs.Write(clientId);
			socket.send(ombs.GetBufferPtr(), ombs.GetByteLength(), serverIP, 5001);
			setUpDone = true;
		}
		else if (PlayerInfo::PacketType::PT_NEWPLAYER) {
			imbs.Read(&dotPosition.x, 4);
			imbs.Read(&dotPosition.y, 4);
			imbs.Read(&receivedId);
			playersList.push_back(new PlayerInfo(receivedId, sf::Vector2i(dotPosition.x * 64, dotPosition.y * 64), sf::Color(255,0,0,150)));

			ombs.Write(PlayerInfo::PacketType::PT_ACK, 3);
			ombs.Write(clientId);
			socket.send(ombs.GetBufferPtr(), ombs.GetByteLength(), serverIP, 5001);
		}
		else if (PlayerInfo::PacketType::PT_PING) {
			ombs.Write(PlayerInfo::PacketType::PT_PING, 3);
			ombs.Write(clientId);
			socket.send(ombs.GetBufferPtr(), ombs.GetByteLength(), serverIP, 5001);
		}
		else if (PlayerInfo::PacketType::PT_DISCONNECT) {
			int enemyID = 0;
			imbs.Read(&enemyID);
			delete playersList[enemyID];
			playersList.erase(playersList.begin() + enemyID);
			for (int i = 0; i < playersList.size(); i++) {
				if (i >= enemyID) playersList[i]->id--;
			}
			if (clientId > enemyID) clientId--;
			std::cout << "Player " << enemyID << " disconnected" << std::endl;
			ombs.Write(PlayerInfo::PacketType::PT_ACK, 3);
			ombs.Write(clientId);
			socket.send(ombs.GetBufferPtr(), ombs.GetByteLength(), serverIP, 5001);
		}
		//Draw window sprites and text
		window.draw(grid);
		for (int i = 0; i < playersList.size(); i++) playersList[i]->Render(window);
		window.display();
		window.clear();
	}
	for (int i = 0; i < playersList.size(); i++) delete playersList[i];
	ombs.Write(PlayerInfo::PacketType::PT_DISCONNECT, 3);;
	socket.send(ombs.GetBufferPtr(), ombs.GetByteLength(), serverIP, 5001);
	socket.unbind();
	return 0;
}