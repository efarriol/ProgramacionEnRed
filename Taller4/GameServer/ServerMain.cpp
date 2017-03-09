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

#define TIME 5

std::string GenerateWord() {

	std::string targetWord;
	int numWords = 0;
	srand(time(NULL));
	std::ifstream file;
	file.open("WordsBasedata.txt");
	if (file.is_open()) {
		file >> numWords;
		int randomWord = rand() % numWords;
		for (int i = 0; i < randomWord; i++) {
			file >> targetWord;
		}
	} file.close();
	return targetWord;
}

void InitGame(std::vector<sf::TcpSocket*> &playerSockets, PlayerInfo &player1, PlayerInfo &player2, std::string &targetWord) {
	sf::Packet sendPacket;
	targetWord = GenerateWord();
	sendPacket.clear();
	sendPacket << player1.name << targetWord << TIME;
	playerSockets[1]->send(sendPacket);
	sendPacket.clear();
	sendPacket << player2.name << targetWord << TIME;
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
	sf::Clock deltaClock;
	sf::Time deltaTime;

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
					InitGame(playerSocket, players[0], players[1], targetWord);
					deltaClock.restart();
				}
			}
		}
		else {
			deltaTime = deltaClock.getElapsedTime();
			for (int i = 0; i < 2; i++) {
				statusReceive = playerSocket[i]->receive(packet);
				if (statusReceive == sf::Socket::Done) {
					packet >> message >> isAnswer;
					deltaTime = deltaClock.getElapsedTime();
					if (isAnswer && deltaTime.asSeconds() < TIME) {
						if (message == targetWord) {
							players[i].score += 10;
							message += " - correcto";
							targetWord = GenerateWord();
							//restart time
							deltaClock.restart();
						}
						else {
							message += " - error";
						}
					}
					packet.clear();
					packet << players[i].name << players[i].score << message << TIME - (int)deltaTime.asSeconds() << isAnswer << targetWord;
					for (int j = 0; j < 2; j++) {
						//sends to all if is an answer or only to the opponent if is input text
						if (!isAnswer && j != i || isAnswer) playerSocket[j]->send(packet);
					}
				}
				else if (statusReceive == sf::Socket::Disconnected) {
					playerSocket[i]->disconnect();
				}
				if (deltaTime.asSeconds() > TIME) {
					deltaClock.restart();
					deltaTime = deltaClock.getElapsedTime();
					targetWord = GenerateWord();
					packet.clear();
					packet << std::string("") <<  players[i].score << std::string("") << TIME - (int)deltaTime.asSeconds() << isAnswer << targetWord;
					for (int j = 0; j < 2; j++) playerSocket[j]->send(packet);
				}
			}
		}
}
	listener.close();
	return 0;
}


