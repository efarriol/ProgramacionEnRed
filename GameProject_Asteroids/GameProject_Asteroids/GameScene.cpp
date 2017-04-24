#include "Logger.h"
#include "GameScene.h"
#include "NetworkManager.h"
#include <InputOutputMemory\InputMemoryBitStream.h>
#include <InputOutputMemory\InputMemoryStream.h>
#include <InputOutputMemory\OutputMemoryBitStream.h>
#include <InputOutputMemory\OutputMemoryStream.h>

using namespace Logger;



GameScene::GameScene(void) :	backButton(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 1.4f + 50, SCREEN_WIDTH*0.27f, SCREEN_HEIGHT*0.12f, string("BACK")),
								continueButton(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2.5f + 50, SCREEN_WIDTH*0.27f, SCREEN_HEIGHT*0.12f, string("CONTINUE")),
								exitButton(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 1.97f + 50, SCREEN_WIDTH*0.27f, SCREEN_HEIGHT*0.12f, string("EXIT")),
								exitButton2(SCREEN_WIDTH - 100, SCREEN_HEIGHT - 50, SCREEN_WIDTH*0.27f, SCREEN_HEIGHT*0.12f, string("EXIT")) {
	currentState = LOBBY;
}


void GameScene::Setup() {
	player = new Player(Vector2D(SCREEN_WIDTH/2, SCREEN_HEIGHT/2), 30, 39, playerLifes);
	asteroidsManager = new AsteroidsManager(numAsteroids, *player, asteroidsVelocity, incrementalSpeed, targetLevelAsteroid);
	ovniManager = new OvniManager(*player, ovniSpeed, ovniSpawnTime);
	inGameMenu = false;
}

GameScene::~GameScene(void) {
	delete player;
	delete asteroidsManager;
}

void GameScene::OnEntry(void) {
}

void GameScene::OnExit(void) {
}

void GameScene::Update(void) {
	
	static MouseCoords mouseCoords(0, 0);
	if (IM.IsMouseDown<MOUSE_BUTTON_LEFT>()) {
		mouseCoords = IM.GetMouseCoords();
	}
	else mouseCoords.x = 0;

	
		switch (currentState)
		{

		case LOBBY:
			ReadFromFile("./../res/lvl/medium.xml");
			Setup();
			if (NM.ConnectionEstablishment()) {
				currentState = PLAY;
			}
			if (exitButton2.ClickButton(mouseCoords.x, mouseCoords.y)) NM.Disconnect();
			break;
		case PLAY:
			if (IM.IsKeyDown<KEY_BUTTON_ESCAPE>())inGameMenu = true;
			//ovniManager->Update(asteroidsManager->GetLevel());
			asteroidsManager->Update();
			player->Update(TM.GetDeltaTime() / 100000);
			break;
		};
	
	if(inGameMenu) {
		if (continueButton.ClickButton(mouseCoords.x, mouseCoords.y)) {
			inGameMenu = false; 
			mouseCoords.x = 0;
		}
		if (exitButton.ClickButton(mouseCoords.x, mouseCoords.y)) NM.Disconnect();		
	}
}



void GameScene::ReadFromFile(std::string path)
{
	rapidxml::xml_document<> doc;
	std::ifstream file(path);
	if (file.is_open()) {
		std::stringstream buffer;
		buffer << file.rdbuf();
		std::string content(buffer.str());
		doc.parse<0>(&content[0]);

		//std::cout << "Reading:" << doc.first_node()->name() << "\n";
		rapidxml::xml_node<> *pRoot = doc.first_node();

		for (rapidxml::xml_node<> *pNode = pRoot->first_node("attribute"); pNode; pNode = pNode->next_sibling()) {

			//std::cout << pNode->name() << ':' << '\n';
			for (rapidxml::xml_node<> *pNodeI = pNode->first_node(); pNodeI; pNodeI = pNodeI->next_sibling()) {

				std::string attribute = pNodeI->name();
				std::string value = pNodeI->value();

				if (attribute == "velocity")asteroidsVelocity = stof(value, NULL);
				else if (attribute == "numAsteroids") numAsteroids = stof(value, NULL);
				else if (attribute == "incrementalSpeed") incrementalSpeed = stof(value, NULL);
				else if (attribute == "targetLevelAsteroid") targetLevelAsteroid = stof(value, NULL);
				else if (attribute == "ovniSpeed") ovniSpeed = stof(value, NULL);
				else if (attribute == "ovniSpawnTime") ovniSpawnTime = stof(value, NULL);
				else if (attribute == "lifes") playerLifes = stof(value, NULL);
				//std::cout << pNodeI->name() << ':' << pNodeI->value() << '\n';
			}
		}
	}
}
void GameScene::Draw(void)
{
	switch (currentState) {
	case PLAY:
		GUI::DrawTextBlended<FontID::HYPERSPACE>(std::to_string(player->score),
		{ 100, 40, 1, 1 },
		{ 255, 255, 255 });

		for (int i = 0; i < player->lifes; i++) {
			lifeCounter = { { 20 + i * 50, 100, 30, 39 }, 0, ObjectID::PLAYER };
			lifeCounter.Draw();
		}

		asteroidsManager->Draw();
		//ovniManager->Draw();
		player->Draw();
		if (inGameMenu) {
			GUI::DrawTextBlended<FontID::HYPERSPACE>("PAUSE",
			{ SCREEN_WIDTH / 2 - 75, 135, 2, 2 },
			{ 255, 255, 255 });

			continueButton.Draw();
			exitButton.Draw();
		}
		break;

	case LOBBY:
		GUI::DrawTextBlended<FontID::HYPERSPACE>("ASTEROIDS",
		{ SCREEN_WIDTH / 2 - 135, 135, 2, 2 },
		{ 255, 255, 255 });
		GUI::DrawTextBlended<FontID::HYPERSPACE>("WAITING FOR THE OTHER PLAYER...",
		{ SCREEN_WIDTH / 2 + 20, 400, 1, 1},
		{ 255, 255, 255 });
		exitButton2.Draw();
		break;
	}
}