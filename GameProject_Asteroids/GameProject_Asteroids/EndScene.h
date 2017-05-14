#pragma once
#include "Scene.h"
#include "Button.h"
#include "Constants.h"
#include "System.h"
#include <SFML\Network.hpp>

class EndScene : public Scene {
public:
	void Draw(void) override;
	void OnEntry(void) override;
	void OnExit(void) override;
	void Update(void) override;
	Button exitButton;
	EndScene();
	~EndScene();
};

