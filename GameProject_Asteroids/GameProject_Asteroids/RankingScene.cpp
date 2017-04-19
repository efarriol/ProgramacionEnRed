#include "RankingScene.h"

RankingScene::RankingScene():exitButton(SCREEN_WIDTH -100, SCREEN_HEIGHT - 50, SCREEN_WIDTH*0.27f, SCREEN_HEIGHT*0.12f, string("EXIT")) 
{
}


RankingScene::~RankingScene()
{
}

void RankingScene::OnEntry(void)
{
}

void RankingScene::OnExit(void)
{
}

void RankingScene::Update(void)
{
	static MouseCoords mouseCoords(0, 0);
	if (IM.IsMouseDown<MOUSE_BUTTON_LEFT>()) {
		mouseCoords = IM.GetMouseCoords();
	}
	if(!isRead)ReadRanking();
	if (exitButton.ClickButton(mouseCoords.x, mouseCoords.y))exit(0);
}

void RankingScene::Draw(void)
{
	GUI::DrawTextBlended<FontID::HYPERSPACE>("RANKING",
	{ SCREEN_WIDTH / 2 - 130, 25, 2, 2 },
	{ 255, 255, 255 });
	
	for (int i = 0; i < 10; i++) {
		GUI::DrawTextBlended<FontID::HYPERSPACE>(namePool[i],
		{ SCREEN_WIDTH / 2 - 200, 175 + i*60, 1, 1 },
		{ 255, 255, 255 });

		GUI::DrawTextBlended<FontID::HYPERSPACE>(std::to_string(scorePool[i]),
		{ SCREEN_WIDTH / 2 + 200, 175 + i*60, 1, 1 },
		{ 255, 255, 255 });
	}
	exitButton.Draw();
}

void RankingScene::ReadRanking()
{
	fstream rankingFile("./../res/lvl/ranking.dat", std::fstream::in || ios::binary);
	std::string tempName;
	int tempScore;
	if (rankingFile.is_open()) {
		for (int i = 0; i < 10; i++){
			rankingFile >> tempName >> tempScore;
			namePool.push_back(tempName);
			scorePool.push_back(tempScore);
		}
	}
	rankingFile.close();
	isRead = true;
}


