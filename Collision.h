#ifndef COLLISION_H
#define COLLISION_H

#include "SFML/Graphics.hpp" 

class BouncingThing;

class Collision {
public: 
	static float getDotProduct(const sf::Vector2f& v1, const sf::Vector2f& v2);
	static void normalise(sf::Vector2f& v);
	static float getLength(sf::Vector2f v);
	static bool checkCollision(BouncingThing& b1, BouncingThing& b2);
	static bool checkBoundingCircle(BouncingThing& b1, BouncingThing& b2);
	static bool checkForCollisionSAT(BouncingThing& b1, BouncingThing& b2);
	static void resolveCollision(BouncingThing& b1, BouncingThing& b2);
	static bool useSAT;
	static int dotProductCount;
};

#include "BouncingThing.h"

#endif