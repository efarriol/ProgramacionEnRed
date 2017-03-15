#pragma once
#include <SFML\Graphics.hpp>
#include <iostream>

#define TIME 5
#define MAX_WORDS 10
class PlayerInfo
{
public:

	std::string name;
	std::vector<std::string> messages;
	std::string input;
	int score;
	bool isReady;
	PlayerInfo(std::string _name) : name(_name) {
		score = 0;
	};
	PlayerInfo() {
		std::cout << "Enter your name \n";
		std::cin >> name;
		score = 0;
	}
	~PlayerInfo();

};