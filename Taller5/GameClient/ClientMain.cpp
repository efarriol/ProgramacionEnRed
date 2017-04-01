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
	sf::IpAddress serverIP = sf::IpAddress::getLocalAddress();
	sf::IpAddress senderIP;
	unsigned short senderPort;
	sf::UdpSocket socket;
	sf::Packet sendPacket;
	sf::Packet receivePacket;
	socket.setBlocking(false);

	//Init impact dots vector
	sf::Vector2i dotPosition;
	std::vector<sf::CircleShape> playersDot;
	sf::CircleShape dot(32);
	dot.setFillColor(sf::Color(0,255,0,150));
	dot.setPosition(5000, 5000);
	playersDot.push_back(dot);
	sf::String message;
	
	int playersOnline = 0;
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
		sf::sleep(sf::milliseconds(50));
		receivePacket.clear();
		window.pollEvent(evento);

		socket.receive(receivePacket, senderIP, senderPort);
		receivePacket >> receiveMessage;

		if (sendMessage != "Done") {
			socket.send(sendPacket, serverIP, 5001);
			if (receiveMessage == "Welcome") {
				receivePacket >> player1.position.x >> player1.position.y >> playersOnline;
				playersDot[0].setPosition(player1.position.x, player1.position.y);
				for (int i = 0; i < playersOnline; i++) {
					receivePacket >> dotPosition.x >> dotPosition.y;
					sf::CircleShape newDot(32);
					newDot.setFillColor(sf::Color(255, 0, 0, 150));
					newDot.setPosition(dotPosition.x, dotPosition.y);
					playersDot.push_back(newDot);
				}
				sendPacket.clear();
				sendMessage = "Done";
				sendPacket << sendMessage;
				socket.send(sendPacket, serverIP, 5001);
			}
		}
		else if (receiveMessage == "NewPlayer") {
			receivePacket >> dotPosition.x >> dotPosition.y;
			sf::CircleShape newDot(32);
			newDot.setFillColor(sf::Color(255, 0, 0, 150));
			newDot.setPosition(dotPosition.x, dotPosition.y);
			playersDot.push_back(newDot);
		}
		//Draw window sprites and text
		window.draw(grid);
		for (int i = 0; i < playersDot.size(); i++) window.draw(playersDot[i]);
		window.display();
		window.clear();
	}

	return 0;
}
