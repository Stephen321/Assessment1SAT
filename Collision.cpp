#include "Collision.h"
#include <iostream> //test

float Collision::getDotProduct(sf::Vector2f v1, sf::Vector2f v2){
	return (v1.x * v2.x) + (v1.y * v2.y);
}

sf::Vector2f Collision::getNormal(sf::Vector2f v) {
	float length = sqrt((v.x * v.x) + (v.y * v.y));
	return sf::Vector2f(v.x / length, v.y / length);
}

float Collision::getLength(sf::Vector2f v){
	return std::sqrt(((v.x * v.x) + (v.y * v.y)));
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
	sf::Vector2f vectorBetween = b2.getCentre() - b1.getCentre();
	float dot = getDotProduct(vectorBetween, smallestDistNorm);
	std::cout << "Dot: " << dot << std::endl;
	std::cout << "b1.X: " << b1.getCentre().x << std::endl;
	std::cout << "b2.X: " << b2.getCentre().x << std::endl;
	
	if (dot > 0)
		mtv = smallestDistance * -smallestDistNorm;
	else
		mtv = smallestDistance * smallestDistNorm;
	b1.move(mtv);
	return true;
}