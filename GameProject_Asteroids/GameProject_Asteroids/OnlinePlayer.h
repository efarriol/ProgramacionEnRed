#pragma once
#include "System.h"
#include <math.h>
#include "Constants.h"
#include "Entities.h"

class OnlinePlayer : public Entities
{
public:
	OnlinePlayer(Vector2D _position, int _width, int _height, int _lifes) {
		position = _position;
		width = _width;
		height = _height;
		entitieSprite = { { int(position.x), int(position.y), width, height }, 0, ObjectID::PLAYER };
		score = 0;
		lifes = _lifes;
	};
	~OnlinePlayer();
	void Update(Vector2D _position, float angle);
	int lifes;
	int score;
};

