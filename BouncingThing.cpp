#include "BouncingThing.h"

BouncingThing::BouncingThing(float radius, float speed, int screenWidth, int screenHeight, sf::Vector2f position, float shapeRotateSpeed, int shapePoints) : Radius(radius), Speed(speed), ScreenWidth(screenWidth),
ScreenHeight(screenHeight), m_position(position), m_rotationSpeed(shapeRotateSpeed), m_PointAmount(shapePoints) {
	sf::Vector2f direction = sf::Vector2f((float)(((rand() % 201) / 100.0) - 1), (float)(((rand() % 201) / 100.0) - 1)); //-1.00 -> 1.00
	m_velocity = direction * Speed;
	m_rotation = 0;
	m_shape.setPointCount(m_PointAmount);
	m_shape.setOrigin(radius, radius);
	setUpShape();
}

void BouncingThing::setUpShape() {
	float angleIncrement = 360 / (float)m_PointAmount;
	for (int i = 0; i < m_PointAmount; i++){
		float a = ((angleIncrement * i) + m_rotation) * (m_PI / 180);
		m_shape.setPoint(i, m_position + sf::Vector2f((Radius * cos(a)),
													   (Radius * sin(a)))); 
	}
}

void BouncingThing::Update(float dt){
	//CheckWallCollisions();
//	m_position += m_velocity * dt;
//	m_rotation += m_rotationSpeed;
//setUpShape();
}

void BouncingThing::CheckWallCollisions(){
	//wall conditions
	if (m_position.x < Radius || m_position.x + 2 * Radius > ScreenWidth) {
		m_velocity.x *= -1;
		m_position.x += m_velocity.x > 0 ? 1 : -1;
	}
	if (m_position.y < Radius || m_position.y + 2 * Radius > ScreenHeight){
		m_velocity.y *= -1;
		m_position.y += m_velocity.y > 0 ? 1 : -1;
	}
}

bool BouncingThing::isCollidingWith(BouncingThing &c){ 
	bool colliding = false;
	if (this != &c) {
		sf::Vector2f vectorBetween = m_position - c.getPosition();
		float distance = Collision::getLength(vectorBetween);
		if (distance < Radius + c.getRadius()) //Circle collision detection - Broad Phase
			colliding = Collision::checkForCollisionSAT((*this), c); //narrow phase
	}
	return colliding;
}

void BouncingThing::resolveCollisionWith(BouncingThing &c){
	sf::Vector2f vectorBetween = Collision::getNormal(m_position - c.getPosition()); //normalise
	sf::Vector2f vpa1 = Collision::getDotProduct(m_velocity, vectorBetween) * vectorBetween;//parallel proj of velocity onto LOI
	sf::Vector2f vpr1 = m_velocity - vpa1;//perpendicular proj of velocity on LOI (wont change)
	sf::Vector2f vpa2 = Collision::getDotProduct(c.getVelocity(), vectorBetween) * vectorBetween;
	sf::Vector2f vpr2 = c.getVelocity() - vpa2;

	setVelocity(vpa2 + vpr1);
	c.setVelocity(vpa1 + vpr2);
}

sf::Vector2f BouncingThing::getVelocity(){
	return m_velocity;
}

void BouncingThing::setVelocity(sf::Vector2f newVel){
	m_velocity = newVel;
}

sf::ConvexShape BouncingThing::getShape(){
	return m_shape;
}

float BouncingThing::getRadius(){
	return Radius;
}

sf::Vector2f BouncingThing::getPosition(){
	return m_position;
}

void BouncingThing::setRotation(float rotate) {
	m_rotationSpeed = rotate;
}


std::vector<sf::Vector2f> BouncingThing::getNormals(){
	std::vector<sf::Vector2f> normals;
	for (int i = 0; i < m_PointAmount; i++){
		int next;
		if (i == m_PointAmount - 1)
			next = 0;
		else
			next = i + 1;
		float dx = m_shape.getPoint(i).x - m_shape.getPoint(next).x;
		float dy = m_shape.getPoint(i).y - m_shape.getPoint(next).y;

		//This removes duplicates of normals that are just pointing in opposite directions
	/*	for (int j = 0; j < normals.size(); j++){
			if (abs((int)dx) == abs((int)normals[j].y) &&
				abs((int)dy) == abs((int)normals[j].x)) {
				normals.erase(normals.begin() + j);
				break;
			}
		}*/

		normals.push_back(Collision::getNormal(sf::Vector2f(-dy, dx)));
	}
	return normals;
}

std::vector<sf::Vector2f> BouncingThing::getPoints(){
	std::vector<sf::Vector2f> points;
	for (int i = 0; i < m_PointAmount; i++){
		points.push_back(m_shape.getPoint(i));
	}
	return points;
}

void BouncingThing::move(sf::Vector2f value){
	m_position += value;
}