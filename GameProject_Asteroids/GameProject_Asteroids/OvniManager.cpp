#include "OvniManager.h"



OvniManager::OvniManager(Player& _player, float _ovniSpeed, float _ovniSpawnTime)
{
	player = &_player;
	ovniSpeed = _ovniSpeed;
	ovniSpawnTime = _ovniSpawnTime;
	timeLapse = 0;
	numOvnis = 1;
	ovniPool = new Ovni[0];
}


OvniManager::~OvniManager()
{
	delete[] ovniPool;
}

void OvniManager::Update(int level)
{
	for (int i = 0; i < numOvnis; i++) {
		ovniPool[i].Update(TM.GetDeltaTime() / 100000);
		CollisionController(ovniPool[i]);
		ovniPool[i].speed = ovniSpeed + level / 1000;
		ovniPool[i].playerPosition = player->GetPosition();
	}
	timeLapse += TM.GetDeltaTime()/100000;
	if (timeLapse >= ovniSpawnTime) {
		CreateOvni();
		timeLapse = 0;
	}
}

void OvniManager::CollisionController(Ovni & currentOvni)
{

	if (!player->inmortal) {
		Vector2D distanceFrom;
		float distance;
		//Player COllisions
		distanceFrom.x = player->GetPosition().x - currentOvni.GetPosition().x;
		distanceFrom.y = player->GetPosition().y - currentOvni.GetPosition().y;
		
		distance = distanceFrom.Length();
		if (distance <= PLAYER_RADIUS + OVNI_RADIUS) {
			player->lifes--;
			player->Reset();
		}
		//Bullet Collisions
		if (currentOvni.bullet.isActive) {

			distanceFrom.x = currentOvni.bullet.GetPosition().x - player->GetPosition().x;
			distanceFrom.y = currentOvni.bullet.GetPosition().y - player->GetPosition().y;
			distance = distanceFrom.Length();
			if (distance <= PLAYER_RADIUS) {
				player->lifes--;
				player->Reset();
			}
		}
		Bullets* currentBullet;

		//Ovni-Bullet Collision
		for (int i = 0; i < MAX_BULLETS; i++) {
			currentBullet = &player->GetCurrentBullet(i);
			if (currentBullet->isActive) {
				distanceFrom.x = currentBullet->GetPosition().x - currentOvni.GetPosition().x;
				distanceFrom.y = currentBullet->GetPosition().y - currentOvni.GetPosition().y;
				distance = distanceFrom.Length();
				if (distance <= OVNI_RADIUS) {
					player->score += 200;
					currentBullet->Reset();
					DeleteOvni(currentOvni);
				}
			}
		}
		
	}
}

void OvniManager::DivideOvni()
{

}

void OvniManager::CreateOvni()
{
	numOvnis++;

	Ovni* temp = new Ovni[numOvnis];
	for (int i = 0; i < numOvnis; i++) {
		temp[i] = ovniPool[i];
	}
	temp[numOvnis - 1].id = numOvnis - 1;
	temp[numOvnis - 1].playerPosition = player->GetPosition();
	temp[numOvnis - 1].speed = ovniSpeed;
	temp[numOvnis - 1].Setup();
	ovniPool = temp;
}

void OvniManager::DeleteOvni(Ovni & currentOvni)
{
	int it = 0;
	bool eliminated = false;
	numOvnis--;
	if (numOvnis > 0) {
		Ovni* temp = new Ovni[numOvnis];
		for (int i = 0; i < numOvnis; i++) {
			if (i == currentOvni.id) {
				it++;
				eliminated = true;
			}
			temp[i] = ovniPool[it];
			if (eliminated) temp[i].id--;
			it++;
		}
		ovniPool = temp;
	}
	else {
		currentOvni.Setup();
		numOvnis++;
	}
}

void OvniManager::Draw()
{
	if (numOvnis > 0) {
		for (int i = 0; i < numOvnis; i++) {
			//ovniPool[i].Draw();
		}
	}
}
