#include "OnlinePlayer.h"


OnlinePlayer::~OnlinePlayer()
{
}

void OnlinePlayer::UpdatePosition(std::vector<sf::Vector3i> &absolutePositions, float &lambda)
{
	if (absolutePositions.size() > 1) {
		sf::Vector2f vector;
		int angle;
		playerAngle = absolutePositions[0].z;
		vector.x = absolutePositions[1].x - absolutePositions[0].x;
		vector.y = absolutePositions[1].y - absolutePositions[0].y;

		lambda += LAMBDA_CONST;

		position.x = absolutePositions[0].x + lambda*vector.x;
		position.y = absolutePositions[0].y + lambda*vector.y;

		UpdateAngle(absolutePositions[0].z, absolutePositions[1].z, lambda);
		/*if (absolutePositions[0].z >= 0 && absolutePositions[0].z <= 90 &&
			absolutePositions[1].z >= 270 && absolutePositions[1].z <= 360) {
			int newAngle = (absolutePositions[1].z - absolutePositions[0].z)-360;
			angle = absolutePositions[0].z + lambda*(newAngle - absolutePositions[0].z);
		}
		else if (absolutePositions[0].z >= 270 && absolutePositions[0].z <= 360 &&
			absolutePositions[1].z >= 0 && absolutePositions[1].z <= 90) {
			int newAngle = 360 + absolutePositions[1].z;
			angle = absolutePositions[0].z + lambda*(newAngle - absolutePositions[0].z);
		}
		else angle = absolutePositions[0].z + lambda*(absolutePositions[1].z- absolutePositions[0].z);*/

		entitieSprite.transform.x = position.x;
		entitieSprite.transform.y = position.y;
		//entitieSprite.angle = angle;

		if (lambda >= 1.0f) {
			absolutePositions.erase(absolutePositions.begin());
			lambda = 0.0f;
		}
	}
}

void OnlinePlayer::UpdateAngle(int currentAngle, int nextAngle, float &lambda)
{

	if (currentAngle >= 0 && currentAngle <= 90 &&
		nextAngle >= 270 && nextAngle <= 360) {
		int newAngle = (nextAngle - currentAngle) - 360;
		nextAngle = currentAngle + lambda*(newAngle - currentAngle);
	}
	else if (currentAngle >= 270 && currentAngle <= 360 &&
		nextAngle >= 0 && nextAngle <= 90) {
		int newAngle = 360 + nextAngle;
		nextAngle = currentAngle + lambda*(newAngle - currentAngle);
	}
	else nextAngle = currentAngle + lambda*(nextAngle - currentAngle);
	
	entitieSprite.angle = nextAngle;
}
void OnlinePlayer::UpdateAngle(int _angle) {
	entitieSprite.angle = _angle;
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
