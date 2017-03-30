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

	sf::IpAddress ip = sf::IpAddress::getLocalAddress();
	sf::IpAddress senderIP;
	unsigned short senderPort;
	sf::UdpSocket socket;
	sf::Packet packet;
	socket.setBlocking(false);

	sf::Socket::Status status = socket.bind(5001);
	if (status != sf::Socket::Done) {
		std::cout << "Can't bind to port 5000" << std::endl;
	}

	std::string message;
	//Start main loop
	while (true) {
		sf::sleep(sf::milliseconds(50));	
		socket.receive(packet, senderIP, senderPort);
		packet >> message;
		if (message == "hello") {
			message = "fuck";
			packet.clear();
			packet << message;
			socket.send(packet, senderIP, senderPort);
		}
		else if (message == "fuck") {
			message = "hello";
			packet.clear();
			packet << message;
			socket.send(packet, senderIP, senderPort);
		}


	}
}


