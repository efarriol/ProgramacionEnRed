#pragma once
#include <SFML\Graphics.hpp>
#include <SFML\Network.hpp>
#include <string>
#include <cstring>
#include <iostream>
#include <vector>
#include <Windows.h>
#include <PlayerInfo.h>

void InitGame(std::vector<sf::TcpSocket*> &playerSockets, PlayerInfo &player1, PlayerInfo &player2) {
	sf::Packet sendPacket;
	sendPacket << player1.name;
	playerSockets[1]->send(sendPacket);
	sendPacket << player2.name;
	playerSockets[0]->send(sendPacket);
	player1.score = 0;
	player2.score = 0;

}

void CheckSended(sf::Socket::Status &statusReceive, std::string text, sf::TcpSocket &socket, std::size_t &sent) {
	//	while (true) {
	while (statusReceive == sf::Socket::Partial) {
		std::string text2 = text.substr(sent);
		socket.send(text2.c_str(), text.length() + 1, sent);
	}
	//else break;
}

int main()
{
	PlayerInfo players[2] = { "eloi", "pol" };
	int socketCount = 0;
	sf::TcpListener listener;
	sf::Socket::Status statusAccept;
	sf::Socket::Status statusReceive;
	sf::Packet receivePacket;
	sf::Packet sendPacket;

	std::string targetWord;
	std::string message;
	bool isAnswer = false;

	//char data[1300];
//	std::size_t bytesReceived;
	//std::vector<sf::TcpSocket*> socketList;
	//socketList.push_back(new sf::TcpSocket);
	std::vector <sf::TcpSocket*> playerSocket;
	playerSocket.push_back(new sf::TcpSocket);
	playerSocket.push_back(new sf::TcpSocket);


	sf::Socket::Status statusListen;
	//	char buffer[2000];
	//	std::size_t received;
	std::vector<std::string> aMensajes;


	statusListen = listener.listen(5000);
	if (statusListen != sf::Socket::Done) {
		std::cout << "Error: " << statusListen << " al vincular puerto 5000" << "." << std::endl;
		system("pause");
		return 0;
	}

	listener.setBlocking(false);


	sf::String mensaje;

	while (true) {

		sf::sleep(sf::milliseconds(200));
		if (socketCount < 2) {
			statusAccept = listener.accept(*playerSocket[socketCount]);
			if (statusAccept == sf::Socket::Done) {
				playerSocket[socketCount]->setBlocking(false);
				playerSocket[socketCount]->receive(receivePacket);
				receivePacket >> players[socketCount].name;
				std::cout << players[socketCount].name << std::endl;
				socketCount++;
				//if players are connected send opponent's name
				if (socketCount == 2) {
					InitGame(playerSocket, players[0], players[1]);
				}
			}
		}


		else {
			for (int i = 0; i < 2; i++) {
				statusReceive = playerSocket[i]->receive(receivePacket);
				if (statusReceive == sf::Socket::Done) {
					receivePacket >> message >> isAnswer;
					if (isAnswer) {
						if (message == targetWord) {
							players[i].score += 10;
							message += " - correcto";
							//restart time
							//crear nueva palabra
						}
						else {
							message += " - error";
						}
						//receivePacket << players[i].name << players[i].score << message << time;
						for (int j = 0; j < 2; j++) {
							playerSocket[j]->send(receivePacket);
						}
					}
					else {
						for (int j = 0; j < 2; j++) {
							if (j != i)playerSocket[j]->send(receivePacket);
						}
					}
				}

				else if (statusReceive == sf::Socket::Disconnected) {
					playerSocket[i]->disconnect();
					//delete  playerSocket[i];
					//playerSocket.erase(socketList.begin() + i);
					//socketCount--;
				}
			}
		}
}
	listener.close();
//	for (int i = 0; i < socketCount; i++) socketList[i]->disconnect();
	return 0;
}


