#pragma once
#include <SFML\Graphics.hpp>
#include <SFML\Network.hpp>
#include <string>
#include <cstring>
#include <iostream>
#include <vector>
#include <Windows.h>
#include <PlayerInfo.h>



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
			}
		}


		//if (p1StatusAccept == sf::Socket::Done && p2StatusAccept == sf::Socket::Done) {
		//	for (int i = 0; i < 2; i++) {



		//		//statusReceive = playerSocket[i].receive(data, 1300, bytesReceived);

		//		if (statusReceive == sf::Socket::Done) {
		//			//	std::string str = data;
		//			//	aMensajes.push_back(str);
		//			for (int j = 0; j < 2; j++) {
		//				//					size_t send;
		//				//					playerSocket[j].send(str.c_str(), str.length() + 1, send);
		//				//					CheckSended(statusReceive, str, playerSocket[j], send);
		//			}
		//		}

		//		else if (statusReceive == sf::Socket::Disconnected) {
		//			playerSocket[i]->disconnect();
		//			//delete  playerSocket[i];
		//			//playerSocket.erase(socketList.begin() + i);
		//			//socketCount--;
		//		}
		//	}
		//}
}
	listener.close();
//	for (int i = 0; i < socketCount; i++) socketList[i]->disconnect();
	return 0;
}


