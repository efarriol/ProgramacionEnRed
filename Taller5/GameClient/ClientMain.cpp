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
	sf::Socket::Status status = socket.bind(5000);
	if (status != sf::Socket::Done) {
		std::cout << "Can't bind to port 5000" << std::endl;
	}

	//Init impact dots vector
	std::vector<sf::CircleShape> playersDot;
	sf::String message;
	
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


	//Init Windows
	sf::Vector2i screenDimensions(640, 640);
	sf::RenderWindow window;
	window.create(sf::VideoMode(screenDimensions.x, screenDimensions.y), "Taller 05");

	//Init events
	sf::Event evento;
	sf::Mouse mouseEvent;
	std::string receiveMessage;

	//Request name
	std::cin >> player1.name;

	sendPacket << "Hello" << player1.name;

	//Start GameLoop
	while (true) {
		receivePacket.clear();
		sf::sleep(sf::milliseconds(50));
		window.pollEvent(evento);

		socket.send(sendPacket, serverIP, 5001);

		socket.receive(receivePacket, senderIP, senderPort);
		receivePacket >> receiveMessage;
		std::cout << receiveMessage << std::endl;
		if (receiveMessage == "fuck") {
			sendPacket.clear();
			sendPacket << "ok";
		}
		

		//Draw window sprites and text
		window.draw(grid);
		window.display();
		window.clear();
	}

	return 0;
}
