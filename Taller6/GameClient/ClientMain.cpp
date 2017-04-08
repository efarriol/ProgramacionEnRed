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
	//PlayerInfo player1;
	std::vector <PlayerInfo *> playersList;
	sf::IpAddress serverIP = sf::IpAddress::getLocalAddress();
	sf::IpAddress senderIP;
	unsigned short senderPort;
	sf::UdpSocket socket;
	sf::Packet sendPacket;
	sf::Packet receivePacket;
	socket.setBlocking(false);

	//Init impact dots vector
	sf::Vector2i dotPosition;
	sf::String message;
	
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
	sf::Text messageText("", font, 45);
	messageText.setStyle(sf::Text::Bold);
	messageText.setPosition(300, 700);

	//Request name
	//std::cout << "Introduce your name:" << std::endl;
	//std::cin >> player1.name;
	std::string sendMessage = "Hello";
	sendPacket << sendMessage << NULL;

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
		receivePacket.clear();
		window.pollEvent(evento);

		socket.receive(receivePacket, senderIP, senderPort);
		receivePacket >> receiveMessage;
		sf::sleep(sf::milliseconds(50));
		if (evento.key.code == sf::Keyboard::Escape) break;
		
		if (sendMessage != "Done") {
			socket.send(sendPacket, serverIP, 5001);
			if (receiveMessage == "Welcome") {
				int playersCount = 0;
				receivePacket >> playersCount;
				for (int i = 0; i < playersCount; i++) {
					receivePacket >> dotPosition.x >> dotPosition.y >> receivedId;
					playersList.push_back(new PlayerInfo(receivedId, dotPosition, sf::Color(255,0,0,150)));
				}
				receivePacket >> dotPosition.x >> dotPosition.y >> receivedId;
				playersList.push_back(new PlayerInfo(receivedId, dotPosition, sf::Color(0,255,0,150)));
				clientId = receivedId;
				sendPacket.clear();
				sendMessage = "Done";
				sendPacket << sendMessage << clientId;
				socket.send(sendPacket, serverIP, 5001);
			}
		}
		else if (receiveMessage == "NewPlayer") {
			receivePacket >> dotPosition.x >> dotPosition.y >> receivedId;
			playersList.push_back(new PlayerInfo(receivedId, dotPosition, sf::Color(255,0,0,150)));
		}
		else if (receiveMessage == "PING") {
			sendPacket.clear();
			sendPacket << "PONG" << clientId;
			socket.send(sendPacket, serverIP, 5001);
		}
		else if (receiveMessage == "PlayerDisconnected") {
			int enemyID = 0;
			receivePacket >> enemyID;
			delete playersList[enemyID];
			playersList.erase(playersList.begin() + enemyID);
			for (int i = 0; i < playersList.size(); i++) {
				if (i >= enemyID) playersList[i]->id--;
			}
			if (clientId > enemyID) clientId--;
			std::cout << "Player " << enemyID << " disconnected" << std::endl;
		}
		//Draw window sprites and text
		window.draw(grid);
		for (int i = 0; i < playersList.size(); i++) playersList[i]->Render(window);
		window.display();
		window.clear();
	}
	sendPacket.clear();
	for (int i = 0; i < playersList.size(); i++) delete playersList[i];
	sendMessage = "Disconnection";
	sendPacket  << sendMessage << clientId;
	socket.send(sendPacket, serverIP, 5001);
	socket.unbind();
	return 0;
}