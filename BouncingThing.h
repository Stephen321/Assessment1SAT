#ifndef BOUNCINGTHING_H
#define BOUNCINGTHING_H

#include "Collision.h"

class BouncingThing {

public:
	BouncingThing(float radius = 10, float speed = 5, int screenWidth = 800, int screenHeight = 600,
		sf::Vector2f position = sf::Vector2f(), float shapeRotateSpeed = 0, int shapePoints = 16);
	void Update(float);
	bool isCollidingWith(BouncingThing &c);
	void resolveCollisionWith(BouncingThing &c);
	sf::ConvexShape getShape();
	int getID();
	sf::Vector2f getVelocity();
	sf::Vector2f getCentre();
	float getRadius();
	void setRotation(float rotate);
	//testing
	sf::Vector2f getPosition();
	std::vector<sf::Vector2f> getNormals();
	std::vector<sf::Vector2f> getPoints();
	void move(sf::Vector2f value);

protected:
	void CheckWallCollisions();
	void setVelocity(sf::Vector2f newVel);
	sf::ConvexShape m_shape;
	sf::Vector2f m_velocity;
	sf::Vector2f m_position;
	const float Speed;
	const float Radius;
	const int ScreenWidth;
	const int ScreenHeight;
	float  m_rotationSpeed;
	float m_rotation;
	int const m_PointAmount;
	float const m_PI = 3.1415926535f;
	void setUpShape();
};


#endif