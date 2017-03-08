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
	while (statusReceive == sf::Socket::Partial) {
		std::string text2 = text.substr(sent);
		socket.send(text2.c_str(), text.length() + 1, sent);
	}
}

void SendFunction(sf::TcpSocket &socket, std::vector<std::string> &aMensajes,
	sf::RenderWindow &window, sf::Event& evento, sf::String &mensaje, std::string &name, sf::Socket::Status &statusReceive) {

	std::string text;
	std::size_t sent;

	int nameSize = name.size() + 5;

	while (window.pollEvent(evento)) {
		switch (evento.type)
		{
		case sf::Event::Closed:
			window.close();
			break;
		case sf::Event::KeyPressed:
			if (evento.key.code == sf::Keyboard::Escape) {
			
				text = " > " + name + ": " + "/exit";
			}
			else if (evento.key.code == sf::Keyboard::Return) {
				if (mensaje.getSize() > nameSize) {

					if (evento.key.code == sf::Keyboard::Return && mensaje.getSize() > nameSize) {
						text = mensaje;
					}
					else text = "";
					if (text.length() > 0 && text != " > " + name + ": " + "/exit")
					{
						mensaje = " > " + name + ": ";
						socket.send(text.c_str(), text.length() + 1, sent);
						CheckSended(statusReceive, text, socket, sent);
					}
				}
			}
			if (text == " > " + name + ": " + "/exit")
			{
				text = "----- " + name + " has left the chat -----";
				socket.send(text.c_str(), text.length() + 1, sent);
				CheckSended(statusReceive, text, socket, sent);
				socket.disconnect();
				exit(0);
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
	sf::Socket::Status statusReceive;
	sf::Socket::Status statusListen;
	char buffer[2000];
	char data[1300];
	std::size_t bytesReceived;
	std::size_t received;
	std::vector<std::string> aMensajes;
	int nameSize;
	tcpSocket->setBlocking(false);
	PlayerInfo p1;
	
	std::string toSend =  p1.name;
	sf::Packet packetToSend;
	packetToSend << p1.name;
	
	tcpSocket->connect(ip, 5000);
	//statusReceive = tcpSocket->receive(data, 1300, bytesReceived);
	tcpSocket->send(packetToSend);
	//CheckSended(statusReceive, toSend, *tcpSocket, received);

	sf::Vector2i screenDimensions(800, 600);

	sf::RenderWindow window;
	window.create(sf::VideoMode(screenDimensions.x, screenDimensions.y), "The Fastest Writer");
	sf::Font font;
	if (!font.loadFromFile("courbd.ttf"))
	{
		std::cout << "Can't load the font file" << std::endl;
	}

	sf::String mensaje = " > " + p1.name + ": ";

	sf::Text userText(mensaje, font, 14);
	userText.setFillColor(sf::Color(0, 160, 0));
	userText.setStyle(sf::Text::Bold);

	sf::Text opponentText(mensaje, font, 14);
	opponentText.setFillColor(sf::Color(0, 160, 0));
	opponentText.setStyle(sf::Text::Bold);

	sf::Text userInput(mensaje, font, 14);
	userInput.setFillColor(sf::Color(0, 160, 0));
	userInput.setStyle(sf::Text::Bold);
	userInput.setPosition(0, 560);

	sf::Text opponentInput("_", font, 14);
	opponentInput.setFillColor(sf::Color(0, 160, 0));
	opponentInput.setStyle(sf::Text::Bold);
	opponentInput.setPosition(415, 560);

	sf::Text userName(p1.name, font, 16);
	userName.setFillColor(sf::Color(200, 200, 200));
	userName.setStyle(sf::Text::Bold);
	userName.setPosition(20, 75);

	sf::Text userScore("0", font, 16);
	userScore.setFillColor(sf::Color(0, 150, 200));
	userScore.setStyle(sf::Text::Bold);
	userScore.setPosition(300, 75);


	sf::Text opponentName("Pol", font, 16);
	opponentName.setFillColor(sf::Color(200, 200, 200));
	opponentName.setStyle(sf::Text::Bold);
	opponentName.setPosition(420, 75);

	sf::Text opponentScore("0", font, 16);
	opponentScore.setFillColor(sf::Color(0, 150, 200));
	opponentScore.setStyle(sf::Text::Bold);
	opponentScore.setPosition(700, 75);

	sf::Text wordToWrite("p2", font, 18);
	wordToWrite.setFillColor(sf::Color(255, 255, 0));
	wordToWrite.setStyle(sf::Text::Bold);
	wordToWrite.setPosition(20, 20);

	sf::Text timer("5555", font, 18);
	timer.setFillColor(sf::Color(200, 50, 0));
	timer.setStyle(sf::Text::Bold);
	timer.setPosition(750, 20);

	//Chat interface
	sf::RectangleShape separator_bottom(sf::Vector2f(800, 5));
	separator_bottom.setFillColor(sf::Color(200, 200, 200, 255));
	separator_bottom.setPosition(0, 550);
	sf::RectangleShape separator_top_h(sf::Vector2f(800, 5));
	separator_top_h.setFillColor(sf::Color(200, 200, 200, 255));
	separator_top_h.setPosition(0, 125);
	sf::RectangleShape separator_top2_h(sf::Vector2f(800, 5));
	separator_top2_h.setFillColor(sf::Color(200, 200, 200, 255));
	separator_top2_h.setPosition(0, 50);
	sf::RectangleShape separator_middle_v(sf::Vector2f(5, 800));
	separator_middle_v.setFillColor(sf::Color(200, 200, 200, 255));
	separator_middle_v.setPosition(400, 50);
	sf::RectangleShape separator_v(sf::Vector2f(5, 50));
	separator_v.setFillColor(sf::Color(200, 200, 200, 255));
	separator_v.setPosition(600, 0);

	sf::Event evento;

	while (true) {

		sf::sleep(sf::milliseconds(100));
		statusReceive = tcpSocket->receive(data, 1300, bytesReceived);

		if (statusReceive == sf::Socket::NotReady) {
			SendFunction(*tcpSocket, aMensajes, window, evento, mensaje, p1.name, statusReceive);
		}
		else if (statusReceive == sf::Socket::Done) {
			std::string str = data;
			aMensajes.push_back(str);
			str = str.substr(0, bytesReceived);
		}
		else if (statusReceive == sf::Socket::Disconnected) aMensajes.push_back(" > The server has closed the party, disconnection in 5 seconds...");


		for (size_t i = 0; i < aMensajes.size(); i++)
		{
			std::string chatting = aMensajes[i];
			userText.setPosition(sf::Vector2f(0, screenDimensions.y*0.25f + 20 * i));
			userText.setString(chatting);
			window.draw(userText);

		}
		for (size_t i = 0; i < aMensajes.size(); i++)
		{
			std::string chatting = aMensajes[i];
			opponentText.setPosition(sf::Vector2f(screenDimensions.x*0.5f, screenDimensions.y*0.25f + 20 * i));
			opponentText.setString(chatting);
			window.draw(opponentText);

		}

		if (aMensajes.size() > 17)
		{
			aMensajes.erase(aMensajes.begin(), aMensajes.begin() + 1);
		}
		window.draw(separator_bottom);
		window.draw(separator_top_h);
		window.draw(separator_top2_h);
		window.draw(separator_middle_v);
		window.draw(separator_v);
		std::string mensaje_ = mensaje + "_";
		userInput.setString(mensaje_);

		window.draw(userInput);
		window.draw(opponentInput);
		window.draw(userName);
		window.draw(userScore);
		window.draw(opponentName);
		window.draw(opponentScore);
		window.draw(wordToWrite);
		window.draw(timer);
		window.display();
		window.clear();

		if (statusReceive == sf::Socket::Disconnected) {
			//Sleep(5000);
			//break;
		}
	}
	tcpSocket->disconnect();
	return 0;
}

