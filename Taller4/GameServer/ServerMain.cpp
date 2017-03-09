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

std::string GenerateWord() {

	std::string targetWord;
	int numWords = 0;
	int i = 0;
	srand(time(NULL));
	std::ifstream file;
	file.open("WordsBasedata.txt");
	if (file.is_open()) {
		file >> numWords;
		int randomWord = rand() % numWords + 1;
		for (i = 0; i < randomWord - 1; i++) {
			file >> targetWord;
		}
	} file.close();
	return targetWord;
}

void InitGame(std::vector<sf::TcpSocket*> &playerSockets, PlayerInfo &player1, PlayerInfo &player2, std::string &targetWord) {
	sf::Packet sendPacket;
	targetWord = GenerateWord();
	sendPacket.clear();
	sendPacket << player1.name << targetWord;
	playerSockets[1]->send(sendPacket);
	sendPacket.clear();
	sendPacket << player2.name << targetWord;
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
	int wordCounter = 0;

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
				socketCount++;
				//if players are connected send opponent's name
				if (socketCount == 2) {
					InitGame(playerSocket, players[0], players[1], targetWord);
					listener.close();
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
					if (isAnswer && deltaTime.asSeconds() < TIME+1) {
						if (message == targetWord) {
							players[i].score += 10;
							message += " - correcto";
							targetWord = GenerateWord();
							wordCounter++;
							//restart time
							deltaClock.restart();
						}
						else {
							message += " - error";
							players[i].score -= 5;
						}
					}
					packet.clear();
					packet << players[i].name << players[i].score << message  << isAnswer << targetWord;
					for (int j = 0; j < 2; j++) {
						//sends to all if is an answer or only to the opponent if is input text
						if (!isAnswer && j != i || isAnswer) playerSocket[j]->send(packet);
					}
				}
				else if (statusReceive == sf::Socket::Disconnected) {
					playerSocket[i]->disconnect();
				}
				if (deltaTime.asSeconds() > TIME+1) {
					deltaClock.restart();
					deltaTime = deltaClock.getElapsedTime();
					targetWord = GenerateWord();
					wordCounter++;
					packet.clear();
					packet << std::string("") <<  players[i].score << std::string("") << isAnswer << targetWord;
					for (int j = 0; j < 2; j++) playerSocket[j]->send(packet);
				}
				if (wordCounter > MAX_WORDS) {
					for (int j = 0; j < 2; j++) {
						packet.clear();
						if (players[0].score > players[1].score) message = "*** The winner is: " + players[0].name + " ***";
						else if (players[1].score > players[0].score) message = "*** The winner is: " + players[1].name + " ***";
						else message = "***** It's a draw!!! *****";
						packet << players[j].name << players[j].score << message << true << "Game Finished";
						playerSocket[j]->send(packet); 
					}
					sf::sleep(sf::milliseconds(5000));
					playerSocket[0]->disconnect();
					playerSocket[1]->disconnect();
					delete playerSocket[0];
					delete playerSocket[1];
					return 0;
				}
			}
		}
	}
}


