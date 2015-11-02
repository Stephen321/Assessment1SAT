#ifndef COLLISION_H
#define COLLISION_H

#include "SFML/Graphics.hpp" 

class BouncingThing;

class Collision {
public: 
	static float getDotProduct(sf::Vector2f v1, sf::Vector2f v2);
	static sf::Vector2f getNormal(sf::Vector2f v);
	static float getLength(sf::Vector2f v);
	static bool checkForCollisionSAT(BouncingThing& b1, BouncingThing& b2);
};

#include "BouncingThing.h"

#endif