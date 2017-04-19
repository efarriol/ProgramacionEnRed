#include "Entities.h"

Entities::Entities()
{
}

Entities::~Entities()
{
}

void Entities::Draw()
{
	entitieSprite.Draw();
}

void Entities::DoWrap(Vector2D & position)
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

Vector2D Entities::GetPosition()
{
	Vector2D centredPosition;
	centredPosition.x = position.x + width / 2;
	centredPosition.y = position.y + height / 2;
	return centredPosition;
}
