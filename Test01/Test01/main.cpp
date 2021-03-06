#include <SFML\Graphics.hpp>
#include <SFML\Network.hpp>
#include <string>
#include <cstring>
#include <iostream>

int main()
{
	sf::IpAddress ip = sf::IpAddress::getLocalAddress();//("192.168.1.106");// sf::IpAddress::getLocalAddress();
	sf::TcpSocket socket;
	char connectionType, mode;
	char buffer[2000];
	std::size_t received;
	std::string text = "Connected to: ";

	std::cout << "Enter (s) for Server, Enter (c) for Client: ";
	std::cin >> connectionType;

	if (connectionType == 's')
	{
		sf::TcpListener listener;
		listener.listen(5000);
		listener.accept(socket);
		text += "Server";
		mode = 's';
		listener.close();
	}
	else if (connectionType == 'c')
	{
		socket.connect(ip, 5000);
		text += "Client";
		mode = 'r';
	}

	socket.send(text.c_str(), text.length() + 1);
	socket.receive(buffer, sizeof(buffer), received);

	std::cout << buffer << std::endl;

	bool done = false;
	while (!done)
	{
		if (mode == 's')
		{
			std::getline(std::cin, text);
			if (text.length() > 0)
			{
				socket.send(text.c_str(), text.length() + 1);
				mode = 'r';
				if (text == "exit")
				{
					break;
				}
			}
		}
		else if (mode == 'r')
		{
			socket.receive(buffer, sizeof(buffer), received);
			if (received > 0)
			{
				std::cout << "Received: " << buffer << std::endl;
				mode = 's';
				if (strcmp(buffer, "exit") == 0)
				{
					break;
				}
			}
		}
	}

	socket.disconnect();
	system("pause");

	return 0;
}
//
//#include <SFML\Graphics.hpp>
//#include <string>
//#include <iostream>
//#include <vector>
//
//#define MAX_MENSAJES 30
//
//
//int main()
//{
//	std::vector<std::string> aMensajes;
//
//	sf::Vector2i screenDimensions(800, 600);
//
//	sf::RenderWindow window;
//	window.create(sf::VideoMode(screenDimensions.x, screenDimensions.y), "Chat");
//
//	sf::Font font;
//	if (!font.loadFromFile("courbd.ttf"))
//	{
//		std::cout << "Can't load the font file" << std::endl;
//	}
//
//	sf::String mensaje = " >";
//
//	sf::Text chattingText(mensaje, font, 14);
//	chattingText.setFillColor(sf::Color(0, 160, 0));
//	chattingText.setStyle(sf::Text::Bold);
//
//
//	sf::Text text(mensaje, font, 14);
//	text.setFillColor(sf::Color(0, 160, 0));
//	text.setStyle(sf::Text::Bold);
//	text.setPosition(0, 560);
//
//	sf::RectangleShape separator(sf::Vector2f(800, 5));
//	separator.setFillColor(sf::Color(200, 200, 200, 255));
//	separator.setPosition(0, 550);
//
//	while (window.isOpen())
//	{
//		sf::Event evento;
//		while (window.pollEvent(evento))
//		{
//			switch (evento.type)
//			{
//			case sf::Event::Closed:
//				window.close();
//				break;
//			case sf::Event::KeyPressed:
//				if (evento.key.code == sf::Keyboard::Escape)
//					window.close();
//				else if (evento.key.code == sf::Keyboard::Return)
//				{
//					aMensajes.push_back(mensaje);
//					if (aMensajes.size() > 25)
//					{
//						aMensajes.erase(aMensajes.begin(), aMensajes.begin() + 1);
//					}
//					mensaje = ">";
//				}
//				break;
//			case sf::Event::TextEntered:
//				if (evento.text.unicode >= 32 && evento.text.unicode <= 126)
//					mensaje += (char)evento.text.unicode;
//				else if (evento.text.unicode == 8 && mensaje.getSize() > 0)
//					mensaje.erase(mensaje.getSize() - 1, mensaje.getSize());
//				break;
//			}
//		}
//		window.draw(separator);
//		for (size_t i = 0; i < aMensajes.size(); i++)
//		{
//			std::string chatting = aMensajes[i];
//			chattingText.setPosition(sf::Vector2f(0, 20 * i));
//			chattingText.setString(chatting);
//			window.draw(chattingText);
//		}
//		std::string mensaje_ = mensaje + "_";
//		text.setString(mensaje_);
//		window.draw(text);
//
//
//		window.display();
//		window.clear();
//	}
//
//}