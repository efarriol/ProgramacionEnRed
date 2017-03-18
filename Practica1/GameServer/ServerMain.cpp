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



int main()
{
	//???????
	sf::Texture blue_grid;
	blue_grid.loadFromFile("./../Resources/Images/blue_grid.png");
	Grid grid(sf::Vector2i(0, 0), blue_grid);
	Grid grid2(sf::Vector2i(0, 0), blue_grid);
	bool firstContact[2]{ true };
	bool gameReady = false;
	firstContact[1] = true;
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
			if (firstContact[i]) {																//If player connects for first time
				statusAccept = listener.accept(*playerSocket[i]);
				if (statusAccept == sf::Socket::Done) {
					playerSocket[i]->setBlocking(false);
					playerSocket[i]->receive(packet);
					packet >> players[i]->name;
					players[i]->hasTurn = 1 * i; //random method to init the turn
					packet.clear();
					packet << i << players[i]->hasTurn;  // "i" refers to the faction (0 ISA, 1 RSF)
					playerSocket[i]->send(packet);
					packet.clear();
					firstContact[i] = false;
				}
			}
			else {
				statusReceive = playerSocket[i]->receive(packet);							 //When client send grid information
				if (statusReceive == sf::Socket::Done) {
					if (!players[i]->isReady) {
						for (int x = 0; x < MAX_CELLS; x++) {
							for (int y = 0; y < MAX_CELLS; y++) {
								int value;
								packet >> value;
								players[i]->grid.SetCell(sf::Vector2i(y, x), value);		//Copy the information to the player grid
							}
						}
						players[i]->isReady = true;

					}
					if (players[0]->isReady && players[1]->isReady) {
						if (!gameReady) {
							gameReady = true;
							packet.clear();
							packet << gameReady;
							playerSocket[0]->send(packet);
							playerSocket[1]->send(packet);
							packet.clear();
						}
						else {

							//empezamos a recibir los disparos
							packet >> players[i]->shotCoords.x >> players[i]->shotCoords.y;

							int opponentIt;
							if (i == 0) opponentIt = 1;
							else opponentIt = 0;

							if (players[opponentIt]->grid.GetCell(players[i]->shotCoords) == 0) { //FAIL
								packet.clear();
								players[i]->hasTurn = false;
								players[i]->isImpact = false;
								players[opponentIt]->hasTurn = true;
								players[opponentIt]->isImpact = false;
								std::cout << "Space" << std::endl;
							}
							else {														//ATTEMPTION IMPACT! ALL PILOTS TO BATTLESTATION, REPEAT, ALL PILOTS TO BATTLESTATION
								packet.clear();
								players[i]->hasTurn = true;
								players[i]->isImpact = true;
								players[opponentIt]->hasTurn = false;
								players[opponentIt]->isImpact = true;
								std::cout << "Impact" << std::endl;
							}
							packet << players[i]->hasTurn << players[i]->isImpact << players[i]->shotCoords.x << players[i]->shotCoords.y;
							playerSocket[i]->send(packet);

							packet.clear();
							packet << players[opponentIt]->hasTurn << players[opponentIt]->isImpact << players[i]->shotCoords.x << players[i]->shotCoords.y;
							playerSocket[opponentIt]->send(packet);
						}
					}
				}
				//else if (statusReceive == sf::Socket::NotReady) {
				//	if (players[0]->isReady && players[1]->isReady) {						//When the two players are ready...
				//		packet.clear();
				//		if (players[i]->hasTurn) {
				//			packet << players[i]->hasTurn;
				//		}

				//		//aqui deberiamos enviar que el player[0] tiene el turno 

				//	}
				//}

			}
		}

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


