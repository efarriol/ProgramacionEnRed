#pragma once
#include <SFML\Graphics.hpp>
#include <iostream>

class PlayerInfo
{
public:

	std::string name;
	std::vector<std::string> messages;
	std::string input;
	int score;

	PlayerInfo(std::string _name) : name(_name) {};
	PlayerInfo() {
		std::cout << "Enter your name \n";
		std::cin >> name;
	}
	~PlayerInfo();

};