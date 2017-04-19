#pragma once
#include "Scene.h"
#include "Button.h"
#include "Constants.h"
#include "Sprite.h"
#include "GameScene.h"

class MenuScene : public Scene{
public:
	explicit MenuScene();
	~MenuScene() override;
	void OnEntry(void) override;
	void OnExit(void) override;
	void Update(void) override;
	void Draw(void) override;
private:
	Button playButton;
	Button rankingButton;
	Button exitButton;


	int m_score{ 0 };
};

