#include <iostream>
#pragma once

// Enum ID used to identify each Game Object in the Game
enum class ObjectID : int {
	BUTTON, 
	BULLET,
	PLAYER, //ENTITIES 
	BIG_ASTEROID1,
	BIG_ASTEROID2,
	BIG_ASTEROID3,
	MEDIUM_ASTEROID1,
	SMALL_ASTEROID1,
	OVNI
};
inline std::ostream &operator<<(std::ostream &os, const ObjectID &id) { return os << int(id); };
inline std::istream &operator >> (std::istream &is, ObjectID &id) { return is >> reinterpret_cast<int&>(id); };

// Enum ID used to store each true type font for text rendering
enum class FontID : int {
	ARIAL,
	HYPERSPACE,
	FACTORY,
	MAX
};