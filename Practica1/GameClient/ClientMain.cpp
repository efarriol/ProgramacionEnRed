#pragma once
#include <SFML\Graphics.hpp>
#include <SFML\Network.hpp>
#include <string>
#include <cstring>
#include <iostream>
#include <vector>
#include <Windows.h>
#include <PlayerInfo.h>

void SendFunction(sf::TcpSocket &socket, std::vector<std::string> &aMensajes,
	sf::RenderWindow &window, sf::Event& evento, sf::String &mensaje, std::string &name, sf::Socket::Status &statusReceive) {

	std::string text;
	std::size_t sent;
	sf::Packet packet;

	while (window.pollEvent(evento)) {
		switch (evento.type)
		{
		case sf::Event::Closed:
			window.close();
			break;
		case sf::Event::KeyPressed:
			if (evento.key.code == sf::Keyboard::Escape) {

				text = "/exit";
			}
			else if (evento.key.code == sf::Keyboard::Return) {
				if (mensaje.getSize()>0) {
					text = mensaje;
				}
				else text = "";
				if (text.length() > 0 && text != "/exit")
				{
					packet << text << true;
					mensaje = "";
					socket.send(packet);
					packet.clear();
				}
			}
			if (text == "/exit")
			{
				text = "----- " + name + " has left the game -----";
				packet.clear();
				packet << text << true;
				socket.send(packet);
				socket.disconnect();
				exit(0);
			}
			break;
		case sf::Event::TextEntered:
			if (evento.text.unicode >= 32 && evento.text.unicode <= 126 && mensaje.getSize() < 35){
				mensaje += (char)evento.text.unicode;
				
			}
			else if (evento.text.unicode == 8 && mensaje.getSize() > 0)
				mensaje.erase(mensaje.getSize() - 1, mensaje.getSize());
			packet.clear();
			text = mensaje;
			packet << text << false;
			socket.send(packet);
			break;
		}
	}
}


int main()
{
	sf::IpAddress ip = sf::IpAddress::getLocalAddress();
	sf::TcpSocket* tcpSocket = new sf::TcpSocket;
	sf::Socket::Status statusReceive;
	tcpSocket->setBlocking(false);
	PlayerInfo p1;
	PlayerInfo p2("");
	p2.name = "";
	bool first = true;
	bool firstWait = false;
	bool isAnswer = true;

	int tempTime = 0;
	int time = 0;
	sf::Clock deltaClock;
	sf::Time deltaTime;
	std::string opponentInput;
	std::string targetWord = "Waiting for the Opponent";
	std::string previusTargetWord;
	sf::Packet sendPacket;
	sf::Packet receivePacket;
	sendPacket << p1.name;
	
	tcpSocket->connect(ip, 5000);
	tcpSocket->send(sendPacket);
	sf::Vector2i screenDimensions(800, 600);

	sf::RenderWindow window;
	window.create(sf::VideoMode(screenDimensions.x, screenDimensions.y), "The Fastest Writer");
	sf::Font font;
	if (!font.loadFromFile("courbd.ttf"))
	{
		std::cout << "Can't load the font file" << std::endl;
	}

	sf::String mensaje;

	sf::Text userText(mensaje, font, 14);
	userText.setFillColor(sf::Color(0, 160, 0));
	userText.setStyle(sf::Text::Bold);
	
	sf::Text opponentText(mensaje, font, 14);
	opponentText.setFillColor(sf::Color(0, 160, 0));
	opponentText.setStyle(sf::Text::Bold);

	sf::Text userInput(mensaje, font, 14);
	userInput.setFillColor(sf::Color(0, 160, 0));
	userInput.setStyle(sf::Text::Bold);
	userInput.setPosition(5, 560);

	sf::Text opponentInputText(opponentInput, font, 14);
	opponentInputText.setFillColor(sf::Color(0, 160, 0));
	opponentInputText.setStyle(sf::Text::Bold);
	opponentInputText.setPosition(415, 560);

	sf::Text userName(p1.name, font, 16);
	userName.setFillColor(sf::Color(200, 200, 200));
	userName.setStyle(sf::Text::Bold);
	userName.setPosition(20, 75);

	sf::Text userScore(std::to_string(p1.score), font, 16);
	userScore.setFillColor(sf::Color(0, 150, 200));
	userScore.setStyle(sf::Text::Bold);
	userScore.setPosition(300, 75);


	sf::Text opponentName(p2.name, font, 16);
	opponentName.setFillColor(sf::Color(200, 200, 200));
	opponentName.setStyle(sf::Text::Bold);
	opponentName.setPosition(420, 75);

	sf::Text opponentScore(std::to_string(p2.score), font, 16);
	opponentScore.setFillColor(sf::Color(0, 150, 200));
	opponentScore.setStyle(sf::Text::Bold);
	opponentScore.setPosition(700, 75);

	sf::Text wordToWrite(targetWord, font, 18);
	wordToWrite.setFillColor(sf::Color(255, 255, 0));
	wordToWrite.setStyle(sf::Text::Bold);
	wordToWrite.setPosition(20, 20);

	sf::Text timer(std::to_string(time), font, 18);
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
		//sf::sleep(sf::milliseconds(100));
		statusReceive = tcpSocket->receive(receivePacket);
		if (statusReceive == sf::Socket::NotReady) {
			SendFunction(*tcpSocket, p1.messages, window, evento, mensaje, p1.name, statusReceive);
		}
		else if (statusReceive == sf::Socket::Done) {
			if (first) {
				receivePacket >> p2.name >> targetWord;
				opponentName.setString(p2.name);
				wordToWrite.setString("Waiting...");
				p1.messages.push_back("Game will start in 5 seconds...");
				deltaClock.restart();
				firstWait = true;
			}
			else {
				std::string inName;
				int inScore;
				std::string inMessage;
				receivePacket >> inName >> inScore >> inMessage >> isAnswer >> targetWord;
				wordToWrite.setString(targetWord);
		
				if (inName.size() > 0) {
					if (inName == p1.name) {
						p1.score = inScore;
						userScore.setString(std::to_string(p1.score));
						if (isAnswer) {
							p1.messages.push_back(inMessage);
						}
					}
					else {
						p2.score = inScore;
						opponentScore.setString(std::to_string(p2.score));
						if (isAnswer)
						{
							p2.messages.push_back(inMessage);
							opponentInput = "_";
							opponentInputText.setString(opponentInput);
						}
						else {
							opponentInput = inMessage + "_";
							opponentInputText.setString(opponentInput);
						}

					}
				}
				else  deltaClock.restart();
				if (previusTargetWord != targetWord) {
					deltaClock.restart();
					previusTargetWord = targetWord;
				}
			}
		}
		else if (statusReceive == sf::Socket::Disconnected)  p1.messages.push_back("Disconnection in 5 seconds...");
		
		//Time management
		deltaTime = deltaClock.getElapsedTime();

		if ((int)deltaTime.asSeconds() != tempTime) {
			time = TIME - (int)deltaTime.asSeconds();
			timer.setString(std::to_string(time));
			tempTime = (int)deltaTime.asSeconds();
		}
		if (time == -1) deltaClock.restart();

		for (size_t i = 0; i <  p1.messages.size(); i++)
		{
			std::string chatting = p1.messages[i];
			userText.setPosition(sf::Vector2f(5, screenDimensions.y*0.25f + 20 * i));
			userText.setString(chatting);
			window.draw(userText);

		}
		for (size_t i = 0; i <  p2.messages.size(); i++)
		{
			std::string chatting = p2.messages[i];
			opponentText.setPosition(sf::Vector2f(screenDimensions.x*0.5f+10.0f, screenDimensions.y*0.25f + 20 * i));
			opponentText.setString(chatting);
			window.draw(opponentText);

		}
		
		if (p1.messages.size() > 17)
		{
			p1.messages.erase(p1.messages.begin(), p1.messages.begin() + 1);
		}
		if (p2.messages.size() > 17)
		{
			p2.messages.erase(p2.messages.begin(), p2.messages.begin() + 1);
		}


		window.draw(separator_bottom);
		window.draw(separator_top_h);
		window.draw(separator_top2_h);
		window.draw(separator_middle_v);
		window.draw(separator_v);
		std::string mensaje_ = mensaje + "_";
		userInput.setString(mensaje_);

		window.draw(userInput);
		window.draw(opponentInputText);
		window.draw(userName);
		window.draw(userScore);
		window.draw(opponentName);
		window.draw(opponentScore);
		window.draw(wordToWrite);
		window.draw(timer);
		window.display();
		window.clear();

		if (firstWait) {
			sf::sleep(sf::milliseconds(5000));
			p1.messages.push_back("GO!!");
			firstWait = false;
			first = false;
		}


		if (statusReceive == sf::Socket::Disconnected) {
			sf::sleep(sf::milliseconds(5000));
			break;
		}
	}
	tcpSocket->disconnect();
	delete tcpSocket;
	return 0;
}


