#pragma once
#include <SFML\Graphics.hpp>
#include <SFML\Network.hpp>
#include <string>
#include <cstring>
#include <iostream>
#include <vector>
#include <Windows.h>

#define MAX_MENSAJES 30
sf::Mutex mutex;

void CheckSended(sf::Socket::Status &statusReceive, std::string text, sf::TcpSocket &socket, std::size_t &sent) {
	while (true) {
		if (statusReceive == sf::Socket::Partial) {
			std::string text2 = text.substr(sent);
			socket.send(text2.c_str(), text.length() + 1, sent);
		}
		else break;
	}
}

int main()
{
	int socketCount = 0;
	sf::TcpListener listener;
	sf::Socket::Status statusAccept;
	std::vector<sf::TcpSocket*> socketList;
	socketList.push_back(new sf::TcpSocket);


	sf::Socket::Status statusListen; 
	char buffer[2000];
	std::size_t received;
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

		statusAccept = listener.accept(*socketList[socketCount]);
		if (statusAccept == sf::Socket::Done) {
			socketList.push_back(new sf::TcpSocket);
			socketList[socketCount]->setBlocking(false);
			socketCount++;
		}

		for (int i = 0; i < socketCount; i++) {
			
			char data[1300];
			std::size_t bytesReceived;
			
			sf::Socket::Status statusReceive = socketList[i]->receive(data, 1300, bytesReceived);

			if (statusReceive == sf::Socket::Done) {
				std::string str = data;
				aMensajes.push_back(str);
				for (int j = 0; j < socketCount; j++) { 
					size_t send;
					socketList[j]->send(str.c_str(), str.length() + 1); 
					CheckSended(statusReceive, str, *socketList[j], send);
				}
			}

			else if (statusReceive == sf::Socket::Disconnected) {
				socketList[i]->disconnect();
				delete socketList[i];
				socketList.erase(socketList.begin() + i);
				socketCount--;
			}
		}
	}

	listener.close();
	for(int i = 0; i < socketCount; i++) socketList[i]->disconnect();
	return 0;
}


