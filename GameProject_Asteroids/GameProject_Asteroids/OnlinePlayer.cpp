#include "OnlinePlayer.h"

OnlinePlayer::~OnlinePlayer()
{
}

void OnlinePlayer::Update(Vector2D _position, float angle)
{
	position = _position;
	entitieSprite.transform.x = position.x;
	entitieSprite.transform.y = position.y;
	entitieSprite.angle = angle;
}
