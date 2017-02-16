#pragma once
#include "Framework.h"
class Receive {

public:
	Receive(sf::TcpSocket &_socket, std::size_t &_received, std::vector<std::string> &_aMensajes, char &_mode) {
		socket = &_socket;
		received = &_received;
		aMensajes = &_aMensajes;
		mode = &_mode;
	};
	sf::TcpSocket *socket;
	std::size_t *received;
	std::vector<std::string> *aMensajes;
	char *mode;

	void ReceiveFunction();
};

void Receive::ReceiveFunction() {

	char buffer[2000];
	while (true) {
		socket->receive(buffer, sizeof(buffer), *received);
		if (received > 0)
		{
			aMensajes->push_back(buffer);
			*mode = 's';
			//if (strcmp(buffer, "exit") == 0)
			//{
			//	break;
			//}
		}
	}
	
}