#include "Bullets.h"

Bullets::~Bullets()
{
}

void Bullets::Update(float deltaTime, float angle)
{
	if (isActive) {
		UpdatePosition(angle);
		lifeTime += deltaTime;
		if (position.x < 0 || position.x > SCREEN_WIDTH ||
			position.y < 0 || position.y >SCREEN_HEIGHT) Reset();
		//if (lifeTime >= 0.02f) Reset();
	}
}

void Bullets::UpdatePosition(float angle)
{
	
	if (firstShoot) {
		velocityDirection.x = MAX_SPEED*sin(angle);
		velocityDirection.y = MAX_SPEED*cos(angle);
	}
	position.x += velocityDirection.x;
	position.y -= velocityDirection.y;

	entitieSprite.transform.x = position.x;
	entitieSprite.transform.y = position.y;

}


void Bullets::Draw()
{
	if(isActive)entitieSprite.Draw();
}

void Bullets::SetActive(bool _isActive) {
	isActive = _isActive;
}

void Bullets::setPosition(Vector2D _position)
{
	position = _position;
}

void Bullets::Reset()
{
	position = 0;
	isActive = false;
	speedCounter = 0;
	lifeTime = 0;
}

