#include "Button.h"
#pragma once

Button::~Button()
{
}

bool Button::ClickButton(float mousePositionX, float mousePositionY)
{
	if (mousePositionX >= positionX && mousePositionX <= positionX + sizeX && mousePositionY >= positionY && mousePositionY <= positionY + sizeY) return true;
	return false;
}

void Button::Draw() {
	GUI::DrawTextBlended<FontID::HYPERSPACE>(text,
	{ positionX + sizeX / 2, positionY + sizeY / 2, 1, 1 },
	{ 255, 255, 255 });
}
