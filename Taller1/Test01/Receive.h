#pragma once
#include "Framework.h"
class Receive {

public:
	Receive(sf::TcpSocket &_socket, std::size_t &_received, std::vector<std::string> &_aMensajes,std::string &_name, sf::RenderWindow &_window) {
		socket = &_socket;
		received = &_received;
		aMensajes = &_aMensajes;
		window = &_window;
	};
	sf::TcpSocket *socket;
	std::size_t *received;
	std::vector<std::string> *aMensajes;
	std::string *name;
	sf::RenderWindow *window;

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
			if (aMensajes->size() > 25)
			{
				aMensajes->erase(aMensajes->begin(), aMensajes->begin() + 1);
			}

			if (strcmp(buffer, "Disconnected")==0)
			{
				socket->disconnect();
				exit(0);
			}
		}
	}
	
}