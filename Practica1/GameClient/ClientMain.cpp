#pragma once
#include <SFML\Graphics.hpp>
#include <SFML\Network.hpp>
#include <string>
#include <cstring>
#include <iostream>
#include <vector>
#include <Windows.h>
#include <PlayerInfo.h>

void SendFunction(sf::TcpSocket &socket, std::vector<std::string> &aMensajes,
	sf::RenderWindow &window, sf::Event& evento, sf::String &mensaje, std::string &name, sf::Socket::Status &statusReceive) {

	std::string text;
	std::size_t sent;
	sf::Packet packet;

}


int main()
{
	sf::IpAddress ip = sf::IpAddress::getLocalAddress();
	sf::TcpSocket* tcpSocket = new sf::TcpSocket;
	sf::Socket::Status statusReceive;
	tcpSocket->setBlocking(false);
	PlayerInfo p1;
	PlayerInfo p2("");
	p2.name = "";
	bool first = true;
	bool firstWait = false;
	bool isAnswer = true;

	int tempTime = 0;
	int time = 0;
	sf::Clock deltaClock;
	sf::Time deltaTime;
	std::string opponentInput;
	std::string targetWord = "Waiting for the Opponent";
	std::string previusTargetWord;
	sf::Packet sendPacket;
	sf::Packet receivePacket;
	sendPacket << p1.name;
	
	tcpSocket->connect(ip, 5000);
	tcpSocket->send(sendPacket);
	sf::Vector2i screenDimensions(1920, 1080);

	sf::RenderWindow window;
	window.create(sf::VideoMode(screenDimensions.x, screenDimensions.y), "Galactic Spaceship - Revenge of the mecha-Putin");
	sf::Font font;
	if (!font.loadFromFile("courbd.ttf"))
	{
		std::cout << "Can't load the font file" << std::endl;
	}

	//Init Textures
	sf::Texture ship_txt;
	ship_txt.loadFromFile("./../Resources/Images/Spaceships.png");

	sf::Texture blue_grid;
	blue_grid.loadFromFile("./../Resources/Images/blue_grid.png");
	sf::Sprite grid1;
	grid1.setTexture(blue_grid);
	grid1.setPosition(0, 0);
	grid1.setScale(1, 1);

	sf::Texture red_grid;
	red_grid.loadFromFile("./../Resources/Images/red_grid.png");
	sf::Sprite grid2;
	grid2.setTexture(red_grid);
	grid2.setPosition(640, 0);
	grid2.setScale(1, 1);
	std::vector<sf::Sprite> fleet;


	sf::String mensaje;
	sf::Event evento;
	sf::Mouse mouseEvenet;
	while (true) {
		window.pollEvent(evento);
		
		if (mouseEvenet.isButtonPressed(sf::Mouse::Left)) {
			sf::Sprite ship;
			ship.setTexture(ship_txt);
			ship.setTextureRect(sf::IntRect(0, 0, 64, 246));
			ship.setPosition(evento.mouseButton.x, evento.mouseButton.y);
			ship.setScale(1, 1);
			fleet.push_back(ship);
		}
		
		//if (statusReceive == sf::Socket::NotReady) {
		//}
		//else if (statusReceive == sf::Socket::Done) {
		//}
		//else if (statusReceive == sf::Socket::Disconnected) {

		//}
		window.draw(grid1);
		window.draw(grid2);
		for (int i = 0; i < fleet.size(); i++) window.draw(fleet[i]);
		window.display();
		window.clear();
	}

	return 0;
}


