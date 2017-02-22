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


void SendFunction(sf::TcpSocket &socket, std::vector<std::string> &aMensajes,
	sf::RenderWindow &window, sf::Event& evento, sf::String &mensaje, std::string &name, sf::Socket::Status &statusReceive) {

	std::string text;
	int nameSize = name.size() + 5;

	while (window.pollEvent(evento)) {
		switch (evento.type)
		{
		case sf::Event::Closed:
			window.close();
			break;
		case sf::Event::KeyPressed:
			if (evento.key.code == sf::Keyboard::Escape) {
				mensaje = " > "  + name + ": " + "/exit";
				aMensajes.push_back(mensaje);
				text = mensaje;
				socket.send(text.c_str(), text.length() + 1);

				break;
			}
			else if (evento.key.code == sf::Keyboard::Return) {
				if (mensaje.getSize() > nameSize) {

					aMensajes.push_back(mensaje);
					
					if (evento.key.code == sf::Keyboard::Return && mensaje.getSize() > nameSize) {
						text = mensaje;
					}
					else text = "";
					if (text.length() > 0)
					{
						if (text == " > " + name + ": " + "/exit")
						{
							socket.disconnect();
							exit(0);
						}
						std::size_t sent;
						mensaje = " > " + name + ": ";
						socket.send(text.c_str(), text.length() + 1, sent);
						while (true) {
							if (statusReceive == sf::Socket::Partial) {
								std::string text2 = text.substr(sent);
								socket.send(text2.c_str(), text.length() + 1, sent);
							}
							else break;
						}
					}
				}
			}
			break;
		case sf::Event::TextEntered:
			if (evento.text.unicode >= 32 && evento.text.unicode <= 126)
				mensaje += (char)evento.text.unicode;
			else if (evento.text.unicode == 8 && mensaje.getSize() > nameSize)
				mensaje.erase(mensaje.getSize() - 1, mensaje.getSize());
			break;
		}
	}

}



int main()
{
	sf::TcpListener listener;
	sf::IpAddress ip = sf::IpAddress::getLocalAddress();
	sf::TcpSocket* tcpSocket = new sf::TcpSocket;
	sf::Socket::Status statusListen; 
	char connectionType, mode;
	char buffer[2000];
	std::size_t received;
	std::string text2 = "Connected to: ";
	std::string name;
	std::vector<std::string> aMensajes;
	int nameSize;

	
	std::cout << "Enter your name \n";
	std::cin >> name;
	std::cout << "Enter (s) for Server, Enter (c) for Client: ";
	std::cin >> connectionType;

	
	if (connectionType == 's')
	{
		statusListen = listener.listen(5000);
		if (statusListen != sf::Socket::Done) {
			std::cout << "Error: " << statusListen << " al vincular puerto 5000" << "." << std::endl;
			system("pause");
			return 0;
		}
		listener.setBlocking(false);
		std::time_t startTime = time(NULL);
		while (startTime + 10 >= time(NULL)) {
			sf::Socket::Status statusAccept = listener.accept(*tcpSocket);
			if (statusAccept == sf::Socket::Done) break;
		}
		
		text2 += "Server";
		mode = 's';
		listener.close();
	}
	else if (connectionType == 'c')
	{
		tcpSocket->connect(ip, 5000);
		tcpSocket->setBlocking(false);
		text2 += "Client";
		mode = 'r';
	}


	tcpSocket->send(text2.c_str(), text2.length() + 1);
	sf::Vector2i screenDimensions(800, 600);

	sf::RenderWindow window;
	window.create(sf::VideoMode(screenDimensions.x, screenDimensions.y), "Chat");
	sf::Font font;
	if (!font.loadFromFile("courbd.ttf"))
	{
		std::cout << "Can't load the font file" << std::endl;
	}

	
	sf::String mensaje = " > " + name + ": ";

	sf::Text chattingText(mensaje, font, 14);
	chattingText.setFillColor(sf::Color(0, 160, 0));
	chattingText.setStyle(sf::Text::Bold);


	sf::Text text(mensaje, font, 14);
	text.setFillColor(sf::Color(0, 160, 0));
	text.setStyle(sf::Text::Bold);
	text.setPosition(0, 560);

	sf::RectangleShape separator(sf::Vector2f(800, 5));
	separator.setFillColor(sf::Color(200, 200, 200, 255));
	separator.setPosition(0, 550);

	sf::Event evento;


	while (true) {
		tcpSocket->setBlocking(false);
		
		char data[1300];
		std::size_t bytesReceived;
		sf::Socket::Status statusReceive = tcpSocket->receive(data, 1300, bytesReceived);

		if (statusReceive == sf::Socket::NotReady) SendFunction(*tcpSocket, aMensajes, window, evento, mensaje, name, statusReceive);
		
		else if (statusReceive == sf::Socket::Done) {
			std::string str = data;
			aMensajes.push_back(str);
			str = str.substr(0, bytesReceived);
		}
		else if (statusReceive == sf::Socket::Disconnected) aMensajes.push_back(" > Your partner have left the session, disconnection in 5 seconds...");


		for (size_t i = 0; i < aMensajes.size(); i++)
		{
			std::string chatting = aMensajes[i];
			chattingText.setPosition(sf::Vector2f(0, 20 * i));
			chattingText.setString(chatting);
			window.draw(chattingText);

		}
		if (aMensajes.size() > 25)
		{
			aMensajes.erase(aMensajes.begin(), aMensajes.begin() + 1);
		}
		window.draw(separator);
		std::string mensaje_ = mensaje + "_";
		text.setString(mensaje_);

		window.draw(text);


		window.display();
		window.clear();

		
		if (statusReceive == sf::Socket::Disconnected) {
			Sleep(5000);
			break;
		}
	}
	tcpSocket->disconnect();

	return 0;
}


