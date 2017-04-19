#pragma once
#include "Player.h"
#include "Ovni.h"
#include <vector>

class OvniManager
{
public:
	OvniManager(Player &_player, float _ovniSpeed, float _ovniSpawnTime);
	~OvniManager();
	void Update(int level);
	void Draw();

private:
	Player* player;
	Ovni* ovniPool;
	float ovniSpeed;
	float ovniSpawnTime;
	float timeLapse;
	int numOvnis;
	void CollisionController(Ovni& currentOvni);
	void DivideOvni();
	void CreateOvni();
	void DeleteOvni(Ovni &currentOvni);

};

