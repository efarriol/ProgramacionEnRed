#include "EndScene.h"



void EndScene::Draw(void)
{
	GUI::DrawTextBlended<FontID::HYPERSPACE>("PARTNER HAVE BEEN DISCONNECTED",
	{ SCREEN_WIDTH / 2 + 20, 400, 1, 1 },
	{ 255, 255, 255 });
	exitButton.Draw();
}

void EndScene::OnEntry(void)
{
}

void EndScene::OnExit(void)
{
}

void EndScene::Update(void)
{
	static MouseCoords mouseCoords(0, 0);
	if (IM.IsMouseDown<MOUSE_BUTTON_LEFT>()) {
		mouseCoords = IM.GetMouseCoords();
	}
	if (exitButton.ClickButton(mouseCoords.x, mouseCoords.y))exit(0);
}

EndScene::EndScene() :exitButton(SCREEN_WIDTH - 100, SCREEN_HEIGHT - 50, SCREEN_WIDTH*0.27f, SCREEN_HEIGHT*0.12f, string("EXIT")) 
{
}


EndScene::~EndScene()
{
}
