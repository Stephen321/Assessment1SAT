#ifndef BOUNCINGTHING_H
#define BOUNCINGTHING_H

#include "Collision.h"

class BouncingThing {

public:
	BouncingThing(float radius = 10, float speed = 5, int screenWidth = 800, int screenHeight = 600,
		sf::Vector2f position = sf::Vector2f(), float shapeRotateSpeed = 0, int shapePoints = 16);
	void Update(float);
	sf::ConvexShape getShape() const;
	int getID();
	sf::Vector2f getVelocity() const;
	float getRadius() const;
	void setRotation(float rotate);
	sf::Vector2f getPosition() const;
	sf::FloatRect getRect() const;
	std::vector<sf::Vector2f> getNormals() const;
	std::vector<sf::Vector2f> getPoints() const;
	void move(sf::Vector2f value);
	void setVelocity(sf::Vector2f newVel);

private:
	void CheckWallCollisions();
	sf::FloatRect m_rect;
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