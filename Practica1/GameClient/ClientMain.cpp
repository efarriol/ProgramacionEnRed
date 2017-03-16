#pragma once
#include <SFML\Graphics.hpp>
#include <SFML\Network.hpp>
#include <string>
#include <cstring>
#include <iostream>
#include <vector>
#include <Vector4.h>
#include <Windows.h>
#include <PlayerInfo.h>
#include <Ship.h>
#include <Fleet.h>
#include <Grid.h>


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
	bool first = true;


	//Init Textures
	sf::Texture shipTexture;
	shipTexture.loadFromFile("./../Resources/Images/Spaceships.png");

	sf::Texture blue_grid;
	blue_grid.loadFromFile("./../Resources/Images/blue_grid.png");
	Grid grid1(sf::Vector2i(0, 0), blue_grid);

	sf::Texture red_grid;
	red_grid.loadFromFile("./../Resources/Images/red_grid.png");
	Grid grid2(sf::Vector2i(640, 0), red_grid);

	PlayerInfo player1("", ISA, grid1);

	std::cout << "Please, introduce your name: " << std::endl;
	std::cin >> player1.name;


	int tempTime = 0;
	int time = 0;
	sf::Clock deltaClock;
	sf::Time deltaTime;
	std::string opponentInput;
	std::string targetWord = "Waiting for the Opponent";
	std::string previusTargetWord;

	sf::Packet packet;


	sf::Vector2i screenDimensions(1280, 840);

	sf::RenderWindow window;
	window.create(sf::VideoMode(screenDimensions.x, screenDimensions.y), "Galactic Spaceship - Revenge of the mecha-Putin");
	sf::Font font;
	if (!font.loadFromFile("courbd.ttf"))
	{
		std::cout << "Can't load the font file" << std::endl;
	}

	sf::String mensaje;
	sf::Event evento;
	sf::Mouse mouseEvent;

	packet << player1.name;
	tcpSocket->connect(ip, 5000);
	tcpSocket->send(packet);

	while (true) {
		packet.clear();

		if (!player1.isReady) {
			statusReceive = tcpSocket->receive(packet);
			if (statusReceive == sf::Socket::Done) {
				packet >> player1.faction;
				player1.fleet.ChangeFaction((Faction)player1.faction);
			}
			player1.fleet.PlaceFleet(window, evento, mouseEvent, player1.isReady);
		}
		else {
			if (first) {
				packet.clear();
				for (int i = 0; i < MAX_CELLS; i++) {
					for (int j = 0; j < MAX_CELLS; j++) packet << grid1.GetCell(sf::Vector2i(j, i));
				}

				tcpSocket->send(packet);
				packet.clear();
				first = false;
			}
		}

		//if (statusReceive == sf::Socket::NotReady) {
		//}
		//else if (statusReceive == sf::Socket::Done) {
		//}
		//else if (statusReceive == sf::Socket::Disconnected) {

		
		for (int i = 0; i < MAX_CELLS; i++){
			for (int j = 0; j < MAX_CELLS; j++) std::cout << " " << grid1.GetCell(sf::Vector2i(j,i));
			std::cout << std::endl;
		}
		system("cls");

		grid1.Render(window);
		grid2.Render(window);
		player1.fleet.Render(window);
		window.display();
		window.clear();
	}

	return 0;
}


