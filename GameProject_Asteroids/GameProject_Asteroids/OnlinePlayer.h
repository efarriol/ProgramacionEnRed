#pragma once
#include "System.h"
#include <math.h>
#include "Constants.h"
#include "Entities.h"
#include <SFML\Graphics.hpp>
#include <SFML\Network.hpp>
#define FLOATtoINT 100000000
#define LAMBDA_CONST 0.0005f

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
	void UpdatePosition(std::vector<sf::Vector3i> &absolutePositions, float &lambda);
	void UpdateAngle(int currentAngle, int newAngle, float &lambda);
	void UpdateAngle(int _angle);
	void DoWrap(Vector2D & position);
	int id = 0;
	int playerAngle = 0;
	int lifes;
	int score;
};

