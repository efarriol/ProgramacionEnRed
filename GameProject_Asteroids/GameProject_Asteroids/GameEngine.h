#include "System.h"
#include "Constants.h"
#include "MenuScene.h"
#include "GameScene.h"
#include "RankingScene.h"

namespace GameEngine {
	
	void LoadMedia(void) {
		R.LoadFont<FontID::ARIAL>("fnt/arial.ttf", 40);
		R.LoadFont<FontID::HYPERSPACE>("fnt/HyperspaceBold.otf", 50);
		R.LoadTexture<ObjectID::PLAYER>("gfx/player.png");		
		R.LoadTexture<ObjectID::BULLET>("gfx/bullet.png");
		R.LoadTexture<ObjectID::BIG_ASTEROID1>("gfx/bigAsteroid1.png");
		R.LoadTexture<ObjectID::BIG_ASTEROID2>("gfx/bigAsteroid2.png");
		R.LoadTexture<ObjectID::BIG_ASTEROID3>("gfx/bigAsteroid3.png");
		R.LoadTexture<ObjectID::MEDIUM_ASTEROID1>("gfx/mediumAsteroid1.png");
		R.LoadTexture<ObjectID::SMALL_ASTEROID1>("gfx/smallAsteroid1.png");
		R.LoadTexture<ObjectID::OVNI>("gfx/ovni.png");

	}
	void AddScenes(void) {
		SM.AddScene<MenuScene>();
		SM.AddScene<GameScene>();
		SM.AddScene<RankingScene>();
		SM.SetCurScene<MenuScene>();
	}
	
	template<int screenWidth, int screenHeight>
	void Run(std::string &&name) {
		Window::Instance(std::move(name), screenWidth, screenHeight); // Initialize window Singleton instance for the first time
		LoadMedia(); // Loads the resource assets
		AddScenes(); // Loads the scenes
		bool m_isRunning = true; // Decides if the game loop is running
		Scene *&m_curScene(SM.GetCurScene()); // Defines a reference to a pointer that score to the current scene pointer (mindblown)
		while (!IM.HasQuit() && m_isRunning) { // Checks while game's still playable
			TM.FPSBegin(); // Calculates the time difference for deltaTime and FPS limiting purposes
#pragma region GAME_UPDATE
			switch (m_curScene->GetState()) { // Check for the state of the screen
			case SceneState::RUNNING:	IM.Update(); m_curScene->Update(); break; // Updates the InputManager and the current scene
			case SceneState::EXIT:		m_isRunning = false; break; // Triggers an end of the game looping
			case SceneState::SLEEP: default:;
			}
#pragma endregion
#pragma region GAME_DRAW
			TM.FPSEnd([&]() { // Limits the FPS rendering with a lambda function as the parameter that draws the whole game
				if (m_curScene->CheckState<SceneState::RUNNING>()) { // If screen object exists and its state is running
					R.Clear();			// Clear the screen buffer
					m_curScene->Draw(); // Call the draw method of the scene
					R.Render();			// Update the screen buffer with all sprites that were pushed
				}});
#pragma endregion
		}
	}

}