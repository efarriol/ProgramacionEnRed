#pragma once
#include "System.h"
#include <math.h>
#include "Constants.h"
#include "Bullets.h"
#include "RankingScene.h"
#include "Entities.h"

#define MAX_SPEED 0.005f
#define RADIUS 25.0f
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
	Vector2D GetAccumuledMovement();
	void RestartAccumuledMovement();
	float GetAngle();
	void Update(float deltaTime);
	virtual void Draw();
	void Reset();
	Bullets& GetCurrentBullet(int i);
	int id = 0;
	int lifes = 0;
	int score = 0;
	bool inmortal;
	enum ControlState {
		KEYBOARD,
		MOUSE
	};
	ControlState controlState;
	void UpdatePosition(Vector2D confirmatedVelocity);
	void UpdateAngle(float _angle);

private:
	Vector2D desiredVelocity;
	Vector2D previousVelocity;
	Vector2D accumuledMovement;
	float speedCounter;
	float angle;
	bool canShoot;
	float inmortalTime;
	float blinkTime; //parpadeo
	std::string name;
	Bullets* bulletPool;

	MouseCoords mouseCoords;

	void UpdateSpeed(float deltaTime);
	void UpdateAngle();
	void FireWeapon(int bullet);
	int bulletCounter;
	void WriteRanking();
};

