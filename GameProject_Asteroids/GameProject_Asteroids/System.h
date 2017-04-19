#pragma once

// Include shortcut to the View and the Controller systems
#include "Window.h"
#include <string>			// Needed to process error messages
#include <SDL\SDL.h>		// Always needs to be included for an SDL app
#include <SDL\SDL_ttf.h>	// Used for loading fonts and rendering text
#include <SDL\SDL_image.h>	// Used for loading and drawing sprites
#include <SDL\SDL_mixer.h>	// Used for loading and playing audio files
#include "Render.h"
#include "TimeManager.h"
#include "InputManager.h"
#include "SceneManager.h"
#include "Vector2D.h"
#include "Constants.h"
#include <random>
#include <time.h>
#include <iostream>
#include <fstream>