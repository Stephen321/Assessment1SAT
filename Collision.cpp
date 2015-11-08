#include "Collision.h"
#include <iostream> //test

bool Collision::useSAT = true;
int Collision::dotProductCount = 0;

float Collision::getDotProduct(const sf::Vector2f& v1, const sf::Vector2f& v2){
	dotProductCount++;
	return (v1.x * v2.x) + (v1.y * v2.y);
}

void Collision::normalise(sf::Vector2f& v) {
	float length = sqrt((v.x * v.x) + (v.y * v.y));
	v =  sf::Vector2f(v.x / length, v.y / length);
}

float Collision::getLength(sf::Vector2f v){
	return std::sqrt(((v.x * v.x) + (v.y * v.y)));
}

bool Collision::checkCollision(BouncingThing& b1, BouncingThing& b2){
	bool colliding = false;
	if (&b1 != &b2) {
		colliding = Collision::checkBoundingCircle(b1, b2);
		if (colliding && useSAT) //Circle collision detection - Broad Phase
			colliding = Collision::checkForCollisionSAT(b1, b2); //narrow phase
	}
	return colliding;
}

bool Collision::checkBoundingCircle(BouncingThing& b1, BouncingThing& b2){
	sf::Vector2f vectorBetween = b1.getPosition() - b2.getPosition();
	float distance = Collision::getLength(vectorBetween);
	return distance < b1.getRadius() + b2.getRadius();
}

bool Collision::checkForCollisionSAT(BouncingThing& b1, BouncingThing& b2){

	std::vector<sf::Vector2f> b1Normals = b1.getNormals();
	std::vector<sf::Vector2f> b2Normals = b2.getNormals();
	std::vector<sf::Vector2f> normals;
	normals.insert(normals.end(), b1Normals.begin(), b1Normals.end());
	normals.insert(normals.end(), b2Normals.begin(), b2Normals.end());
	std::vector<sf::Vector2f> b1Points = b1.getPoints();
	std::vector<sf::Vector2f> b2Points = b2.getPoints();
	float smallestDistance = 99999.f;
	sf::Vector2f mtv;
	sf::Vector2f smallestDistNorm;

	int normalsCount = normals.size();
	int b1PointsCount = b1Points.size();
	int b2PointsCount = b2Points.size();
	for (int i = 0; i < normalsCount; i++){
		int b1Min, b1Max, b2Min, b2Max;
		std::vector<float> projections;
		for (int j = 0; j < b1PointsCount; j++){
			projections.push_back(getDotProduct(b1Points[j], normals[i]));
		}
		b1Min = *std::min_element(projections.begin(), projections.end());
		b1Max = *std::max_element(projections.begin(), projections.end());
		projections.clear();
		for (int k = 0; k < b2PointsCount; k++){
			projections.push_back(getDotProduct(b2Points[k], normals[i]));
		}
		b2Min = *std::min_element(projections.begin(), projections.end());
		b2Max = *std::max_element(projections.begin(), projections.end());

		if ((b1Max <= b2Min) || (b2Max <= b1Min))//not colliding
			return false;

		if (b1Max > b2Min){
			if (smallestDistance > abs(b1Max - b2Min)){
				smallestDistance = abs(b1Max - b2Min);
				smallestDistNorm = normals[i];
			}
		}
		if (b2Max > b1Min){
			if (smallestDistance > abs(b2Max - b1Min)){
				smallestDistance = abs(b2Max - b1Min);
				smallestDistNorm = normals[i];
			}
		}
		std::cout << smallestDistance << std::endl;
	}
	sf::Vector2f vectorBetween = b2.getPosition() - b1.getPosition();
	float dot = getDotProduct(vectorBetween, smallestDistNorm);
	std::cout << "Dot: " << dot << std::endl;
	mtv = smallestDistance * smallestDistNorm;
	
	if (dot > 0)
		mtv = -mtv;
	b1.move(mtv);
	return true;
}

void Collision::resolveCollision(BouncingThing& b1, BouncingThing& b2){
	sf::Vector2f vectorBetween = b1.getPosition() - b2.getPosition();
	Collision::normalise(vectorBetween); 
	sf::Vector2f vpa1 = Collision::getDotProduct(b1.getVelocity(), vectorBetween) * vectorBetween;//parallel proj of velocity onto LOI
	sf::Vector2f vpr1 = b1.getVelocity() - vpa1;//perpendicular proj of velocity on LOI (wont change)
	sf::Vector2f vpa2 = Collision::getDotProduct(b2.getVelocity(), vectorBetween) * vectorBetween;
	sf::Vector2f vpr2 = b2.getVelocity() - vpa2;
	b1.setVelocity(vpa2 + vpr1);
	b2.setVelocity(vpa1 + vpr2);
}