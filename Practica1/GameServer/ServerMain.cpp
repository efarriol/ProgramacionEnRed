#pragma once
#include <SFML\Graphics.hpp>
#include <SFML\Network.hpp>
#include <string>
#include <cstring>
#include <iostream>
#include <fstream>
#include <vector>
#include <Windows.h>
#include <PlayerInfo.h>

void InitGame(std::vector<sf::TcpSocket*> &playerSockets, PlayerInfo &player1, PlayerInfo &player2) {

}

int main()
{
	//???????
	sf::Texture blue_grid;
	blue_grid.loadFromFile("./../Resources/Images/blue_grid.png");
	Grid grid(sf::Vector2i(0, 0), blue_grid);
	Grid grid2(sf::Vector2i(0, 0), blue_grid);
	bool firstContact[2]{true};
	std::vector<PlayerInfo*> players;
	players.push_back(new PlayerInfo("", ISA, grid));
	players.push_back(new PlayerInfo("", RSF, grid2));


	sf::TcpListener listener;
	sf::Socket::Status statusAccept;
	sf::Socket::Status statusReceive;
	sf::Packet packet;
	int socketCount = 0;

	std::vector <sf::TcpSocket*> playerSocket;
	playerSocket.push_back(new sf::TcpSocket);
	playerSocket.push_back(new sf::TcpSocket);
	sf::Socket::Status statusListen;

	statusListen = listener.listen(5000);
	if (statusListen != sf::Socket::Done) {
		std::cout << "Error: " << statusListen << " al vincular puerto 5000" << "." << std::endl;
		system("pause");
		return 0;
	}

	listener.setBlocking(false);
	sf::Clock deltaClock;
	sf::Time deltaTime;

	while (true) {
		packet.clear();

		for (int i = 0; i < 2; i++) {

			if (firstContact[i]) {
				statusAccept = listener.accept(*playerSocket[i]);
				if (statusAccept == sf::Socket::Done) {
					playerSocket[i]->setBlocking(false);
					playerSocket[i]->receive(packet);
					packet >> players[i]->name;
					packet.clear();
					packet << i;
					playerSocket[i]->send(packet);
					packet.clear();
					firstContact[i] = false;
				}
			}
			else {
				statusReceive = playerSocket[i]->receive(packet);
				if (statusReceive == sf::Socket::Done) {
					if (!players[i]->isReady) {
						for (int x = 0; x < MAX_CELLS; x++) {
							for (int y = 0; y < MAX_CELLS; y++) {
								int value;
								packet >> value;
								players[socketCount]->grid.SetCell(sf::Vector2i(y, x), value);
							}
						}
						players[i]->isReady = true;
					}
					else {
						//Disparos....
					}
				}
				else if (statusReceive == sf::Socket::NotReady) {

				}

			}
		}



		//if (socketCount < 2) {
		//	statusAccept = listener.accept(*playerSocket[socketCount]);
		//	if (statusAccept == sf::Socket::Done) {
		//		playerSocket[socketCount]->setBlocking(false);
		//		playerSocket[socketCount]->receive(packet);
		//		packet >> players[socketCount]->name;
		//		for (int i = 0; i < MAX_CELLS; i++) {
		//			for (int j = 0; j < MAX_CELLS; j++) {
		//				int value;
		//				packet >> value;
		//				players[socketCount]->grid.SetCell(sf::Vector2i(j, i), value);
		//			}
		//		}
		//		socketCount++;
		//		//if players are connected send opponent's name
		//		if (socketCount == 2) {
		//			//InitGame(playerSocket, players[0], players[1]);
		//			listener.close();
		//			deltaClock.restart();
		//		}
		//	}
		//}
		//else {
		//	
		//}

		for (int i = 0; i < MAX_CELLS; i++) {
			for (int j = 0; j < MAX_CELLS; j++) std::cout << " " << players[0]->grid.GetCell(sf::Vector2i(j, i));
			std::cout << std::endl;
		}
		std::cout << std::endl << std::endl;
		for (int i = 0; i < MAX_CELLS; i++) {
			for (int j = 0; j < MAX_CELLS; j++) std::cout << " " << players[1]->grid.GetCell(sf::Vector2i(j, i));
			std::cout << std::endl;
		}
		system("cls");


	}
}


