#pragma once
#include "System.h"
#include "Sprite.h"
#include "Entities.h"
#define MAX_SPEED 0.015f

class Bullets : public Entities
{
public:
	Bullets() {
		width = 0;
		height = 0;
		position = 0;
		entitieSprite = { { 0, 0, 8, 8 }, 0, ObjectID::BULLET };
		isActive = false;
		speedCounter = 0;
		lifeTime = 0;
		firstShoot = false;
	}
	~Bullets();
	void Update(float deltaTIme, float angle);
	virtual void Draw();
	void SetActive(bool _isActive);
	void setPosition(Vector2D _position);
	void Reset();

	float lifeTime;
	bool firstShoot;
	bool isActive;


private:
	Vector2D direction;
	Vector2D velocityDirection;
	float speedCounter;

	//reference to collisions!

	void UpdatePosition(float angle);
};

