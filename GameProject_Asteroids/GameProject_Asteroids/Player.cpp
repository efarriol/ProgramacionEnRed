#include "Player.h"


Player::~Player()
{
	delete[] bulletPool;
}

sf::Vector2i Player::GetAccumuledMovement()
{
	return accumuledMovement;
}

void Player::RestartAccumuledMovement()
{
	accumuledMovement.x = 0.0f;
	accumuledMovement.y = 0.0f;
}

int Player::GetAngle()
{
	int _angle = angle*FLOATtoINT;
	return angle;
}


void Player::Update(float deltaTime)
{
	UpdateSpeed(deltaTime);
	UpdateAngle();
	DoWrap(position);
	//UpdatePosition();
	if (canShoot) {
		if (IM.IsKeyDown<KEY_BUTTON_SPACE>()) {
			FireWeapon(bulletCounter);
			bulletCounter++;
			if (bulletCounter == MAX_BULLETS - 1) bulletCounter = 0;
		}
	}
	for (int i = 0; i < MAX_BULLETS; i++) {
		bulletPool[i].Update(deltaTime, entitieSprite.angle*DEG2RAD);
		bulletPool[i].firstShoot = false;
	}
	if (IM.IsKeyDown<'q'>()) {
		position.x = rand() % SCREEN_WIDTH;
		position.y = rand() % SCREEN_HEIGHT;
	}
	inmortalTime += deltaTime*100;
	blinkTime += deltaTime * 100;
	if (blinkTime >= 0.5f) blinkTime = 0;
	if (inmortalTime >= 2)inmortal = false;

	if (lifes <= 0) {
		WriteRanking();
		SM.SetCurScene<RankingScene>();
	}
}


void Player::UpdateSpeed(float deltaTime) {
	if (IM.IsKeyHold<KEY_BUTTON_UP>() || IM.IsKeyHold<'w'>()){
		if(speedCounter <= MAX_SPEED) speedCounter += (0.7f * deltaTime); 
	}
	else {
		if (speedCounter >= 0.0f)speedCounter -= 0.2f*deltaTime;
	}

	if (IM.IsKeyDown<KEY_BUTTON_UP>() || IM.IsKeyDown<'w'>()) {
		previousVelocity = desiredVelocity;
		angle = entitieSprite.angle;
		speedCounter=speedCounter/3;
	}

	desiredVelocity.x = speedCounter*(sin(angle*DEG2RAD));
	desiredVelocity.y = speedCounter*(cos(angle*DEG2RAD));

	accumuledMovement.x += desiredVelocity.x*FLOATtoINT;
	accumuledMovement.y += desiredVelocity.y*FLOATtoINT;
}


void Player::UpdatePosition(sf::Vector2i _confirmatedMovement){ 
	sf::Vector2f confirmatedMovement;
	confirmatedMovement.x = _confirmatedMovement.x / (int)FLOATtoINT;
	confirmatedMovement.y = _confirmatedMovement.y / (int)FLOATtoINT;

	if (angle >= 0 && angle < 90 ||
		angle >= 270 && angle < 360) {
		position.x += confirmatedMovement.x;
		position.y -= confirmatedMovement.y;
	}
	else if (angle >= 90 && angle < 270) {
		position.x -= confirmatedMovement.x;
		position.y += confirmatedMovement.y;
	}

	entitieSprite.transform.x = position.x;
	entitieSprite.transform.y = position.y;
}

void Player::UpdateAngle(float _angle)
{
	entitieSprite.angle = _angle;
}

void Player::FireWeapon(int bullet)
{
	bulletPool[bullet].firstShoot = true;
	Vector2D circlePosition; //To know the direction of bullet, we make a circle and pick a point 
	circlePosition.x = position.x+width/2 + RADIUS*cos((entitieSprite.angle-90)*DEG2RAD);
	circlePosition.y = position.y+height / 2 + RADIUS*sin((entitieSprite.angle-90)*DEG2RAD);

	bulletPool[bullet].setPosition(circlePosition);
	bulletPool[bullet].SetActive(true);
	bulletPool[bulletCounter].lifeTime = 0;
}

void Player::WriteRanking()
{
	system("cls");
	std::cout << "ENTER YOUR NAME" << std::endl;
	std::cin >> name;

	vector<std::string> tempNames;
	vector<int> tempScores;
	std::string rankName;
	int rankScore;
	bool isStored = false;

	std::ifstream inFile;
	inFile.open("./../res/lvl/ranking.dat", ios::binary);
	if(inFile.is_open())
	for (int i = 0; i < 10; i++) {
		inFile >> rankName >> rankScore;
		if (rankScore < score && !isStored) {
			tempNames.push_back(name);
			tempScores.push_back(score);
			isStored = true;
		}
		tempNames.push_back(rankName);
		tempScores.push_back(rankScore);
	}
	inFile.close();

	std::ofstream outFile;
	outFile.open("./../res/lvl/ranking.dat", ios::binary);
	if (outFile.is_open()) {
		for (int i = 0; i < 10; i++) {
			outFile << tempNames[i] << " " << tempScores[i] << std::endl;
		}

	}
	outFile.close();
}


void Player::UpdateAngle()
{
	mouseCoords = IM.GetMouseCoords();

	if (IM.IsKeyHold<KEY_BUTTON_LEFT>() || IM.IsKeyHold<'a'>()) entitieSprite.angle -= 0.008f;
	if (IM.IsKeyHold<KEY_BUTTON_RIGHT>() || IM.IsKeyHold<'d'>()) entitieSprite.angle += 0.008f;
	
	if (entitieSprite.angle <= 0) entitieSprite.angle = 360;
	else if (entitieSprite.angle >= 360) entitieSprite.angle = 0;
	//UpdateAngle(angle);
}


void Player::Reset()
{
	inmortal = true;
	inmortalTime = 0;
	position.x = SCREEN_WIDTH / 2;
	position.y = SCREEN_HEIGHT / 2;
	speedCounter = 0;
}

Bullets& Player::GetCurrentBullet(int i)
{
	return bulletPool[i];
}

void Player::Draw()
{
	if (inmortal) {
		if (blinkTime >= 0.2f) {
			entitieSprite.Draw();
		}
	}
	else entitieSprite.Draw();
	for (int i = 0; i < MAX_BULLETS; i++) {
		bulletPool[i].Draw();
	}
}

