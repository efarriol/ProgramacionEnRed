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
	sendPacket << player1.name;;
	playerSockets[1]->send(sendPacket);
	sendPacket.clear();
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
	sf::Packet packet;
	int time = 0;
	std::string targetWord;
	std::string message;
	bool isAnswer = false;

	std::vector <sf::TcpSocket*> playerSocket;
	playerSocket.push_back(new sf::TcpSocket);
	playerSocket.push_back(new sf::TcpSocket);
	sf::Socket::Status statusListen;
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
		packet.clear();
		//sf::sleep(sf::milliseconds(200));
		if (socketCount < 2) {
			statusAccept = listener.accept(*playerSocket[socketCount]);
			if (statusAccept == sf::Socket::Done) {
				playerSocket[socketCount]->setBlocking(false);
				playerSocket[socketCount]->receive(packet);
				packet >> players[socketCount].name;
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
				statusReceive = playerSocket[i]->receive(packet);
				if (statusReceive == sf::Socket::Done) {
					packet >> message >> isAnswer;
					std::cout << message << std::endl;
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
					}
					packet.clear();
					packet << players[i].name << players[i].score << message << time << isAnswer;
					for (int j = 0; j < 2; j++) {
						//sends to all if is an answer or only to the opponent if is input text
						if(!isAnswer && j!=i || isAnswer) playerSocket[j]->send(packet);
					}
				}
				else if (statusReceive == sf::Socket::Disconnected) {
					playerSocket[i]->disconnect();
				}
			}
		}
}
	listener.close();
	return 0;
}


