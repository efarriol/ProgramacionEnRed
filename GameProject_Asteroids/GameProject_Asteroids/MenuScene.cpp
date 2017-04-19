#include "MenuScene.h"
#include "Logger.h"
using namespace Logger;


MenuScene::MenuScene(void) : playButton(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2.5f + 50, SCREEN_WIDTH*0.27f, SCREEN_HEIGHT*0.12f, string("PLAY")),
rankingButton(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 1.97f + 50, SCREEN_WIDTH*0.27f, SCREEN_HEIGHT*0.12f, string("RANKING")),
exitButton(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 1.6f + 50, SCREEN_WIDTH*0.27f, SCREEN_HEIGHT*0.12f, string("EXIT")) {}

MenuScene::~MenuScene(void) {
}

void MenuScene::OnEntry(void) {
}

void MenuScene::OnExit(void) {
}

void MenuScene::Update(void) {
	static MouseCoords mouseCoords(0, 0);
	if (IM.IsMouseDown<MOUSE_BUTTON_LEFT>()) {
		mouseCoords = IM.GetMouseCoords();
	}
	if (playButton.ClickButton(mouseCoords.x, mouseCoords.y)) SM.SetCurScene<GameScene>();
	if (rankingButton.ClickButton(mouseCoords.x, mouseCoords.y)) SM.SetCurScene<RankingScene>();
	if (exitButton.ClickButton(mouseCoords.x, mouseCoords.y)) exit(0);
	mouseCoords = { 0, 0 };
}

void MenuScene::Draw(void) {
	GUI::DrawTextBlended<FontID::HYPERSPACE>("ASTEROIDS",
	{ SCREEN_WIDTH/2 - 135, 135, 2, 2 },
	{ 255, 255, 255 });

	playButton.Draw();
	rankingButton.Draw();
	exitButton.Draw();
}