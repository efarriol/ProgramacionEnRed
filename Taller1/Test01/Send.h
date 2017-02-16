#pragma once
#include "Framework.h"

class Send {
public:
	Send(sf::TcpSocket &_socket, std::size_t &_received, std::vector<std::string> &_aMensajes, char &_mode,
		sf::RenderWindow &_window, sf::Event& _evento, sf::String &_mensaje, std::string &_name, std::string &_text2) {
		socket = &_socket;
		received = &_received;
		aMensajes = &_aMensajes;
		mode = &_mode;
		window = &_window;
		evento = &_evento;
		mensaje = &_mensaje;
		name = &_name;
		text2 = &_text2;
	}
	sf::TcpSocket *socket;
	std::size_t *received;
	std::vector<std::string> *aMensajes;
	char *mode;
	sf::RenderWindow *window;
	sf::Event *evento;
	sf::String *mensaje;
	std::string *name;
	std::string *text2;


	void SendFunction();
};

void Send::SendFunction() {

	int nameSize = name->size() + 5;

	while (window->pollEvent(*evento)) {
		switch (evento->type)
		{
		case sf::Event::Closed:
			window->close();
			break;
		case sf::Event::KeyPressed:
			if (evento->key.code == sf::Keyboard::Escape) {
				window->close();
				break;
			}
			else if (evento->key.code == sf::Keyboard::Return) {
				if (mensaje->getSize() > nameSize) {
					aMensajes->push_back(*mensaje);
					if (aMensajes->size() > 25)
					{
						aMensajes->erase(aMensajes->begin(), aMensajes->begin() + 1);
					}
				}
			}
			break;
		case sf::Event::TextEntered:
			if (evento->text.unicode >= 32 && evento->text.unicode <= 126)
				mensaje += (char)evento->text.unicode;
			else if (evento->text.unicode == 8 && mensaje->getSize() > nameSize)
				mensaje->erase(mensaje->getSize() - 1, mensaje->getSize());
			break;
		}
	}

	if (evento->key.code == sf::Keyboard::Return && mensaje->getSize() > nameSize) {
		*text2 = *mensaje;
	}
	else *text2 = "";
	if (text2->length() > 0)
	{
		*mensaje = " > " + *name + ": ";
		socket->send(text2->c_str(), text2->length() + 1);
		//*mode = 'r';
		if (*text2 == "exit")
		{
			//break;
		}
	}
}