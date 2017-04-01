#pragma once
#include <SFML\Graphics.hpp>
#include <SFML\Network.hpp>
#include <string>
#include <cstring>
#include <iostream>
#include <vector>
#include <Windows.h>
#include <PlayerInfo.h>

int main()
{
	//Init udp variables
	PlayerInfo player1;
	std::vector <PlayerInfo *> enemyList;
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
	int playerId = 0;
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
	std::cout << "Introduce your name:" << std::endl;
	//std::cin >> player1.name;
	std::string sendMessage = "Hello";
	sendPacket << sendMessage << player1.name;

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

		if (sendMessage != "Done") {
			socket.send(sendPacket, serverIP, 5001);
			if (receiveMessage == "Welcome") {
				receivePacket >> player1.position.x >> player1.position.y >> player1.id;
				player1.dot.setPosition(player1.position.x, player1.position.y);
				for (int i = 0; i < player1.id; i++) {
					receivePacket >> dotPosition.x >> dotPosition.y >> playerId;
					enemyList.push_back(new PlayerInfo(playerId, dotPosition));
				}
				sendPacket.clear();
				sendMessage = "Done";
				sendPacket << sendMessage << player1.id;
				socket.send(sendPacket, serverIP, 5001);
			}
		}
		else if (receiveMessage == "NewPlayer") {
			receivePacket >> dotPosition.x >> dotPosition.y >> playerId;
			enemyList.push_back(new PlayerInfo(playerId, dotPosition));
		}
		//Draw window sprites and text
		window.draw(grid);
		for (int i = 0; i < enemyList.size(); i++) enemyList[i]->Render(window);
		player1.Render(window);
		window.display();
		window.clear();
	}
	sendPacket.clear();
	sendMessage = "Disconnection";
	sendPacket  << sendMessage << player1.id;
	return 0;
}
