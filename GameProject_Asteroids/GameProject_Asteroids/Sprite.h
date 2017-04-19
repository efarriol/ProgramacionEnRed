#pragma once
#include "ID.h"
#include "Render.h"
#include "Transform.h"

// Sprite class that serves as part of the Model
struct Sprite {
	Transform transform;
	double angle;
	ObjectID objectID;
	void Draw() { R.Push(objectID, transform, angle); };
};