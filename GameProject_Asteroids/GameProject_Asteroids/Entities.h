#pragma once
#include "System.h"
#include "Sprite.h"

class Entities
{
public:
	Entities();
	~Entities();
	
	Sprite entitieSprite;

	Vector2D position;
	int width;
	int height;

	void DoWrap(Vector2D &position);
	Vector2D GetPosition();
	
	void Draw();
};

