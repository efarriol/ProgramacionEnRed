#include <SFML\Graphics.hpp>
#include <SFML\Network.hpp>
#include <string>
#include <cstring>
#include <iostream>
#include <vector>

#define MAX_MENSAJES 30


void Send(sf::TcpSocket &socket, std::size_t &received, std::vector<std::string> &aMensajes, char &mode, 
			sf::RenderWindow &window, sf::Event& evento, sf::String &mensaje, std::string &name, std::string &text2) {
	
	int nameSize = name.size() + 5;

	while (window.pollEvent(evento)) {
		switch (evento.type)
		{
		case sf::Event::Closed:
			window.close();
			break;
		case sf::Event::KeyPressed:
			if (evento.key.code == sf::Keyboard::Escape) {
				window.close();
				break;
			}
			else if (evento.key.code == sf::Keyboard::Return) {
				if (mensaje.getSize() > nameSize) {
					aMensajes.push_back(mensaje);
					if (aMensajes.size() > 25)
					{
						aMensajes.erase(aMensajes.begin(), aMensajes.begin() + 1);
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

	if (evento.key.code == sf::Keyboard::Return && mensaje.getSize() > nameSize) {
		text2 = mensaje;
	}
	else text2 = "";
	if (text2.length() > 0)
	{
		mensaje = " > " + name + ": ";
		socket.send(text2.c_str(), text2.length() + 1);
		mode = 'r';
		if (text2 == "exit")
		{
			//break;
		}
	}
}

void Receive(sf::TcpSocket &socket,std::size_t &received, std::vector<std::string> &aMensajes, char &mode){
	
	char buffer[2000];
	socket.receive(buffer, sizeof(buffer), received);
	if (received > 0)
	{
		aMensajes.push_back(buffer);
		mode = 's';
		//if (strcmp(buffer, "exit") == 0)
		//{
		//	break;
		//}
	}
}


int main()
{

	sf::IpAddress ip = sf::IpAddress::getLocalAddress();//("192.168.23.212");// sf::IpAddress::getLocalAddress();
	sf::TcpSocket socket;
	char connectionType, mode;
	char buffer[2000];
	std::size_t received;
	std::string text2 = "Connected to: ";
	std::string name;
	int nameSize;

	std::cout << "Enter your name \n";
	std::cin >> name;
	std::cout << "Enter (s) for Server, Enter (c) for Client: ";
	std::cin >> connectionType;

	
	if (connectionType == 's')
	{
		sf::TcpListener listener;
		listener.listen(5000);
		listener.accept(socket);
		text2 += "Server";
		mode = 's';
		listener.close();
	}
	else if (connectionType == 'c')
	{
		socket.connect(ip, 5000);
		text2 += "Client";
		mode = 'r';
	}

	socket.send(text2.c_str(), text2.length() + 1);
	socket.receive(buffer, sizeof(buffer), received);
	std::cout << buffer << std::endl;

	std::vector<std::string> aMensajes;

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

	while (window.isOpen())
	{
		sf::Event evento;
		if (mode == 's') {
			Send(socket, received, aMensajes, mode, window, evento, mensaje, name, text2);

		/*	while (window.pollEvent(evento)) {
				switch (evento.type)
				{
				case sf::Event::Closed:
					window.close();
					break;
				case sf::Event::KeyPressed:
					if (evento.key.code == sf::Keyboard::Escape) {
						window.close();
						break;
					}
					else if (evento.key.code == sf::Keyboard::Return) {
						if (mensaje.getSize() > nameSize) {
							aMensajes.push_back(mensaje);
							if (aMensajes.size() > 25)
							{
								aMensajes.erase(aMensajes.begin(), aMensajes.begin() + 1);
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

			if (evento.key.code == sf::Keyboard::Return && mensaje.getSize() > nameSize) {
				text2 = mensaje;
			}
			else text2 = "";
			if (text2.length() > 0)
			{
				mensaje = " > " + name + ": ";
				socket.send(text2.c_str(), text2.length() + 1);
				mode = 'r';
				if (text2 == "exit")
				{
					break;
				}
			}*/
		}
		else if (mode == 'r') {
			Receive(socket, received, aMensajes, mode);

			//socket.receive(buffer, sizeof(buffer), received);
			//if (received > 0)
			//{
			//	aMensajes.push_back(buffer);
			//	mode = 's';
			//	if (strcmp(buffer, "exit") == 0)
			//	{
			//		break;
			//	}
			//}
		}

		for (size_t i = 0; i < aMensajes.size(); i++)
		{
			std::string chatting = aMensajes[i];
			chattingText.setPosition(sf::Vector2f(0, 20 * i));
			chattingText.setString(chatting);
			window.draw(chattingText);

		}
		window.draw(separator);
		std::string mensaje_ = mensaje + "_";
		text.setString(mensaje_);

		window.draw(text);


		window.display();
		window.clear();
		
	}

	socket.disconnect();
	system("pause");

	return 0;
}
