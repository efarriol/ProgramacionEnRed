#pragma once
#include "System.h"
#include <math.h>
#include "Constants.h"
#include "Bullets.h"
#include "RankingScene.h"
#include "Entities.h"
#include <SFML\Graphics.hpp>
#include <SFML\Network.hpp>

#define MAX_SPEED 0.005f
#define RADIUS 25.0f
#define FLOATtoINT 100000000.0f

class Player : public Entities
{

public:
	Player(Vector2D _position, int _width, int _height, int _lifes){
		position = _position;
		width = _width;
		height = _height;
		entitieSprite = { { int(position.x), int(position.y), width, height }, 0, ObjectID::PLAYER };
		speedCounter = 0;
		bulletPool = new Bullets[MAX_BULLETS];
		bulletCounter = 0;
		score = 0;
		canShoot = true;
		inmortal = true;
		inmortalTime = 0;
		lifes = _lifes;
	};
	~Player();
	sf::Vector2i GetAccumuledMovement();
	void RestartAccumuledMovement();
	int GetAngle();
	void Update(float deltaTime);
	virtual void Draw();
	void Reset();
	Bullets& GetCurrentBullet(int i);
	int id = 9;
	int lifes = 0;
	int score = 0;
	bool inmortal;
	enum ControlState {
		KEYBOARD,
		MOUSE
	};
	ControlState controlState;
	void UpdatePosition(sf::Vector2i _confirmatedVelocity, int _angle);
	std::string name;

private:
	Vector2D desiredVelocity = 0;
	Vector2D previousVelocity = 0;
	sf::Vector2i accumuledMovement = sf::Vector2i(0, 0);
	float speedCounter = 0;
	float angle = 0;
	bool canShoot;
	float inmortalTime;
	float blinkTime; //parpadeo
	Bullets* bulletPool;

	MouseCoords mouseCoords;

	void UpdateSpeed(float deltaTime);
	void UpdateAngle();
	void FireWeapon(int bullet);
	int bulletCounter;
	void WriteRanking();
};

