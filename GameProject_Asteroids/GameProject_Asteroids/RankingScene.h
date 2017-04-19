#pragma once
#include "Scene.h"
#include "Button.h"
#include "Constants.h"
#include "System.h"

class RankingScene : public Scene{
public:
	RankingScene();
	~RankingScene();
	void OnEntry(void) override;
	void OnExit(void) override;
	void Update(void) override;
	void Draw(void) override;
private:
	Button exitButton;
	vector<std::string> namePool;
	vector<int> scorePool;
	void ReadRanking();
	bool isRead = false;
};

