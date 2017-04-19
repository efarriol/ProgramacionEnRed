#pragma once
#include "Sprite.h"
#include "GUI.h"

using namespace std;

class Button {
	int positionX, positionY, sizeX, sizeY;
	string text;
public:

	Button(int _positionX, int _positionY, int _sizeX, int _sizeY, string _text) : sizeX(_sizeX), sizeY(_sizeY), text(_text) {
		positionX = _positionX - sizeX / 2;
		positionY = _positionY - sizeY / 2;
	};
	~Button();
	bool ClickButton(float mousePositionX, float mousePositionY);
	void Draw();

};