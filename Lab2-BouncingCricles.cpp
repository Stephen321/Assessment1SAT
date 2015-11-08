//////////////////////////////////////////////////////////// 
// Headers 
//////////////////////////////////////////////////////////// 
#include "stdafx.h" 
#include "BouncingThing.h"
#include "QuadTree.h"

#ifdef _DEBUG 
#pragma comment(lib,"sfml-graphics-d.lib") 
#pragma comment(lib,"sfml-audio-d.lib") 
#pragma comment(lib,"sfml-system-d.lib") 
#pragma comment(lib,"sfml-window-d.lib") 
#pragma comment(lib,"sfml-network-d.lib") 
#else 
#pragma comment(lib,"sfml-graphics.lib") 
#pragma comment(lib,"sfml-audio.lib") 
#pragma comment(lib,"sfml-system.lib") 
#pragma comment(lib,"sfml-window.lib") 
#pragma comment(lib,"sfml-network.lib") 
#endif 
#pragma comment(lib,"opengl32.lib") 
#pragma comment(lib,"glu32.lib") 

#include "SFML/Graphics.hpp" 
#include "SFML/OpenGL.hpp" 
#include <iostream> 
#define _USE_MATH_DEFINES
#include <math.h>


////////////////////////////////////////////////////////////
///Entrypoint of application 
//////////////////////////////////////////////////////////// 

void CheckCollisionsWithQuad(std::vector<BouncingThing> &collidableShapes, QuadTree& quad){
	quad.clear();
	for (int i = 0; i < collidableShapes.size(); i++)
		quad.insert(&collidableShapes[i]);
	std::vector<BouncingThing*> returnedObjects;
	std::vector<BouncingThing*> alreadyCollidedObjects;
	for (int i = 0; i < collidableShapes.size(); i++){
		returnedObjects.clear();
		BouncingThing &bT = collidableShapes[i];
		quad.retrieve(returnedObjects, &bT);
		for (int j = 0; j < returnedObjects.size(); j++){
			if (Collision::checkCollision(bT, *(returnedObjects[j])) &&
				std::find(alreadyCollidedObjects.begin(), alreadyCollidedObjects.end(), returnedObjects[j]) == alreadyCollidedObjects.end()){
				Collision::resolveCollision(bT, *(returnedObjects[j]));
				alreadyCollidedObjects.push_back(&bT);
				break;
			}
		}
	}
}

void CheckCollisions(std::vector<BouncingThing> &collidableShapes) { //Check if 2 shapes are colliding using circle collision detection and resolve them
	int amountOfShapes = collidableShapes.size();
	for (int i = 0; i < amountOfShapes; i++){
		for (int j = i + 1; j < amountOfShapes; j++){
			BouncingThing &bT = collidableShapes[i];
			if (Collision::checkCollision(bT, collidableShapes[j])){
				Collision::resolveCollision(bT, collidableShapes[j]);
				break;
			}
		}
	}
}

bool CheckIfColliding(BouncingThing& bT, std::vector<BouncingThing> &collidableShapes) { //If the area to spawn the shape is clear of other shapes
	bool colliding = false;
	int amountOfShapes = collidableShapes.size();
	for (int i = 0; i < amountOfShapes && !colliding; i++){
		if (Collision::checkCollision(bT, collidableShapes[i])){
			Collision::resolveCollision(bT, collidableShapes[i]);
			colliding = true;
		}
	}
	return colliding;
}

void AddShapes(const sf::String& shapeName, int amountToAdd, std::vector<BouncingThing> & collidables){

	const int MinRadius = 10; //min
	const int MaxRadius = 25; //max
	const float Speed = 200;
	const int ScreenWidth = 800;
	const int ScreenHeight = 600;

	for (int i = 0; i < amountToAdd; i++) {
		float radius = MinRadius + (rand() % (MaxRadius - MinRadius + 1));
		float xPos = 3 * MaxRadius + (rand() % (ScreenWidth - (6 * MaxRadius)));
		float yPos = 3 * MaxRadius + (rand() % (ScreenHeight - (6 * MaxRadius)));
		int dir = 1 - ((rand() % 2) * 2); //dir = 1 or -1
		float rndRotationSpeed = (float)(0.05 + ((rand() % 21) / 100));

		int pointCount = 4;
		if (shapeName == "Triangle"){
			pointCount = 3;
		}
		BouncingThing bT(radius, Speed, ScreenWidth, ScreenHeight, sf::Vector2f(xPos, yPos), rndRotationSpeed * dir, pointCount);
		while (CheckIfColliding(bT, collidables));
		collidables.push_back(bT);
	}
}
int main()
{
	// Create the main window 
	sf::RenderWindow window(sf::VideoMode(800, 600, 32), "Bouncing Things");
	
	QuadTree quad(0, sf::FloatRect(0, 0, 800, 600));

	bool useQuadTree = true, showDebug = true;
	//load a font
	sf::Font font;
	font.loadFromFile("C:\\Windows\\Fonts\\GARA.TTF");

	sf::Text fpsText;
	fpsText.setFont(font);
	fpsText.setStyle(sf::Text::Regular);
	fpsText.setPosition(20, 15);
	fpsText.setCharacterSize(32);

	sf::Text dotProductCountText = fpsText;
	dotProductCountText.setPosition(20, 40);

	sf::Text quadTreeEnabledText = fpsText;
	quadTreeEnabledText.setPosition(20, 70);

	sf::Text satEnabledText = quadTreeEnabledText;
	satEnabledText.setPosition(20, 100);

	sf::Text infoText = satEnabledText;
	infoText.setPosition(10, 0);
	infoText.setCharacterSize(20);
	infoText.setString(std::string("Press Q to toggle Quad Tree. Press S to toggle SAT. Press D to toggle Debug info."));

	//stats
	sf::Clock clock, frameTimer, dotProductClock;
	int fps = 0;
	int dotsPerSecond = 0;
	float frames = 0;
	clock.restart();
	frameTimer.restart();

	const int AmountOfSquares = 10;
	const int AmountOfTriangles = 10;
	std::vector<BouncingThing> collidableShapes;
	int amountOfShapes = AmountOfSquares + AmountOfTriangles;
	
	//add shapes
	AddShapes(sf::String("Square"), AmountOfSquares, collidableShapes);
	AddShapes(sf::String("Triangle"), AmountOfTriangles, collidableShapes);

	float test = 0;
	// Start game loop 
	while (window.isOpen())
	{
		//calculate frames per second
		frames += 1;
		if (clock.getElapsedTime().asSeconds() >= 0.25){
			fps = frames * (1/0.25f);
			frames = 0;
			clock.restart();
		}
		if (dotProductClock.getElapsedTime().asSeconds() >= 0.5f){
			dotsPerSecond = Collision::dotProductCount / 0.5f;
			Collision::dotProductCount = 0;
			dotProductClock.restart();
		}

		// Process events 
		sf::Event Event;
		while (window.pollEvent(Event))
		{
			// Close window : exit 
			if (Event.type == sf::Event::Closed)
				window.close();

			// Escape key : exit 
			if ((Event.type == sf::Event::KeyPressed) && (Event.key.code == sf::Keyboard::Escape))
				window.close();
			if ((Event.type == sf::Event::KeyPressed) && (Event.key.code == sf::Keyboard::Q)){
				useQuadTree = !useQuadTree;
				if (useQuadTree == false)
					quad.clear();
			}
			if ((Event.type == sf::Event::KeyPressed) && (Event.key.code == sf::Keyboard::S))
				Collision::useSAT = !Collision::useSAT;
			if ((Event.type == sf::Event::KeyPressed) && (Event.key.code == sf::Keyboard::D))
				showDebug = !showDebug;
			
		}
		float dt = frameTimer.getElapsedTime().asSeconds();
		frameTimer.restart();

		for (int i = 0; i < amountOfShapes; i++) {
			collidableShapes[i].Update(dt);
		}
		if (useQuadTree)
			CheckCollisionsWithQuad(collidableShapes, quad);
		else
			CheckCollisions(collidableShapes);
		fpsText.setString("Fps: " + std::to_string(fps));
		dotProductCountText.setString("Dot Products: " + std::to_string(dotsPerSecond));
		std::string truefalseText = (Collision::useSAT ? "True" : "False");
		satEnabledText.setString("SAT: " + truefalseText);
		truefalseText = (useQuadTree ? "True" : "False");
		quadTreeEnabledText.setString("QuadTree: " + truefalseText);
	
		//prepare frame
		window.clear();

		//draw frame items
		for (int i = 0; i < amountOfShapes; i++) {
			window.draw(collidableShapes[i].getShape());
		}
		if (showDebug) window.draw(quad);
		window.draw(fpsText);
		window.draw(dotProductCountText);
		window.draw(quadTreeEnabledText);
		window.draw(satEnabledText);
		window.draw(infoText);
		// Finally, display rendered frame on screen 
		window.display();
	} //loop back for next frame

	return EXIT_SUCCESS;
}