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

int main(){

	//Init grids
	sf::Texture blue_grid;
	blue_grid.loadFromFile("./../Resources/Images/blue_grid.png");
	Grid grid(sf::Vector2i(0, 0), blue_grid);
	Grid grid2(sf::Vector2i(0, 0), blue_grid);

	//Bool to control flow of the game
	bool firstContact[2]{ true };
	bool gameReady = false;
	firstContact[1] = true;

	//Init players vector
	std::vector<PlayerInfo*> players;
	players.push_back(new PlayerInfo("", ISA, grid));
	players.push_back(new PlayerInfo("", RSF, grid2));

	sf::String message; //message to send to the clients

	//Init tcp variables
	sf::TcpListener listener;
	sf::Socket::Status statusListen;
	sf::Socket::Status statusAccept;
	sf::Socket::Status statusReceive;
	std::vector <sf::TcpSocket*> playerSocket;
	playerSocket.push_back(new sf::TcpSocket);
	playerSocket.push_back(new sf::TcpSocket);
	sf::Packet packet;
	int socketCount = 0;
	statusListen = listener.listen(5000);
	if (statusListen != sf::Socket::Done) {
		std::cout << "Error: " << statusListen << " al vincular puerto 5000" << "." << std::endl;
		system("pause");
		return 0;
	}

	listener.setBlocking(false);
	
	//Start main loop
	while (true) {

		packet.clear();
		//iteration for the two players
		for (int i = 0; i < 2; i++) {
			//If player connects for first time
			if (firstContact[i]) {
				statusAccept = listener.accept(*playerSocket[i]);
				if (statusAccept == sf::Socket::Done) {
					playerSocket[i]->setBlocking(false);
					playerSocket[i]->receive(packet);
					packet >> players[i]->name;
					players[i]->hasTurn = 1 * i; //"random" method to init the turn
					packet.clear();
					packet << i << players[i]->hasTurn;  // "i" refers to the faction (0 ISA, 1 RSF)
					playerSocket[i]->send(packet);
					packet.clear();
					firstContact[i] = false;
				}
			}
			else {
				//When we receive from the clients
				statusReceive = playerSocket[i]->receive(packet);
				if (statusReceive == sf::Socket::Done) {
					//When client send grid information...
					if (!players[i]->isReady) {
						for (int x = 0; x < MAX_CELLS; x++) {
							for (int y = 0; y < MAX_CELLS; y++) {
								int value;
								packet >> value;
								players[i]->grid.SetCell(sf::Vector2i(y, x), value);	//Copy the information to the player grid
							}
						}
						players[i]->isReady = true; //Now player is ready
					}
					//When two players are ready, game starts
					if (players[0]->isReady && players[1]->isReady) {
						//Send to the players that game has started
						if (!gameReady) {
							gameReady = true;
							packet.clear();
							packet << gameReady;
							playerSocket[0]->send(packet);
							playerSocket[1]->send(packet);
							packet.clear();
						}
						else {
							//Start to receive the players shots
							packet >> players[i]->shotCoords.x >> players[i]->shotCoords.y;
							
							message = "";
							//Create opponents iterators
							int opponentIt;
							if (i == 0) opponentIt = 1;
							else opponentIt = 0;

							//Check the players shots
							if (players[opponentIt]->grid.GetCell(players[i]->shotCoords) == 0) { //When is 0, is water
								packet.clear();
								players[i]->hasTurn = false;	//Lose the turn
								players[i]->isImpact = false;	
								players[opponentIt]->hasTurn = true;	//Opponent have turn now
								players[opponentIt]->isImpact = false;
							}
							else {	//Impact whit ship
								packet.clear();
								players[i]->hasTurn = true;		//Player still having turn
								players[i]->isImpact = true;
								players[opponentIt]->hasTurn = false;
								players[opponentIt]->isImpact = true;

								int boatId = players[opponentIt]->grid.GetCell(players[i]->shotCoords); //Get impacted ship ID 
								players[opponentIt]->fleet.GetShip(boatId).TakeDamage();	//Take damage to that ship
								//If that ship have no live points...
								if (players[opponentIt]->fleet.GetShip(boatId).GetDamage() <= 0) {
									//Then ship destroyed
									players[opponentIt]->currentShips--;
									message = players[opponentIt]->fleet.GetShip(boatId).GetBoatName(players[opponentIt]->fleet.GetShip(boatId).GetType()) 
												+ " SHIP HAS BEEN DESTROYED !";
								}
							}
							//If there are no ships alive, the game is over
							if (players[opponentIt]->currentShips <= 0) {
								message = "GameOver";
							}
							//Put all the information to the packet and send to the player
							packet << players[i]->hasTurn << players[i]->isImpact << players[i]->shotCoords.x << players[i]->shotCoords.y << message;
							playerSocket[i]->send(packet);
							packet.clear();
							//Put all the information to the packet and send to the other player
							packet << players[opponentIt]->hasTurn << players[opponentIt]->isImpact << players[i]->shotCoords.x << players[i]->shotCoords.y << message;
							playerSocket[opponentIt]->send(packet);
						}
					}
				}
				//When player disconnects from the server
				else if (statusReceive == sf::Socket::Disconnected) {
					message = "Disconnection";
					packet << players[i]->hasTurn << players[i]->isImpact << -15 << -15 << message; //the only important variable is "message"
					if (i == 0) playerSocket[1]->send(packet);
					else playerSocket[0]->send(packet);
					//Disconnect and delete sockets  and palyers
					playerSocket[0]->disconnect();
					playerSocket[1]->disconnect();
					delete playerSocket[0], playerSocket[1];
					delete players[0], players[1];
					return 0;
				}
			}
		}
	}
}


