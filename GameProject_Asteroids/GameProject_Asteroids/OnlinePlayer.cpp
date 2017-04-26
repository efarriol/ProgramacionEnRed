#include "OnlinePlayer.h"


OnlinePlayer::~OnlinePlayer()
{
}

void OnlinePlayer::Update(sf::Vector2i _accumulatedMovement, float angle)
{

	sf::Vector2f accumulatedMovement;
	accumulatedMovement.x = _accumulatedMovement.x / FLOATtoINT;
	accumulatedMovement.y = _accumulatedMovement.y / FLOATtoINT;
	
	position.x += accumulatedMovement.x;
	position.y -= accumulatedMovement.y;
	entitieSprite.transform.x = position.x;
	entitieSprite.transform.y = position.y;
	entitieSprite.angle = angle;
	DoWrap(position);
}

void OnlinePlayer::DoWrap(Vector2D & position)
{
	if (position.x > SCREEN_WIDTH + width / 2)
	{
		position.x = -50.0f;
	}
	else if (position.x < -50)
	{
		position.x = SCREEN_WIDTH + width / 2;
	}

	if (position.y > SCREEN_HEIGHT + height / 2)
	{
		position.y = -50.0f;
	}
	else if (position.y < -50.0f)
	{
		position.y = SCREEN_HEIGHT + height / 2;
	}
}
