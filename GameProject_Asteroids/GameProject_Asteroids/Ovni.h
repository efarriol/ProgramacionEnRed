#pragma once
#include "System.h"
#include "Sprite.h"
#include "Bullets.h"
#include "Entities.h"

class Ovni : public Entities
{
public:
	Ovni();
	~Ovni();

	void Setup();
	void Update(float deltaTime);
	virtual void Draw();
	float speed;
	Bullets bullet;
	Vector2D playerPosition;
	void Shoot();
	int id;

private:
	Vector2D randomDirection;
	float bulletAngle;
	void UpdatePosition(float deltaTime);
	void RandomizeDirection();
};

