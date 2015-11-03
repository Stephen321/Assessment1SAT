//////////////////////////////////////////////////////////// 
// Headers 
//////////////////////////////////////////////////////////// 
#include "stdafx.h" 
#include "BouncingThing.h"

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

void CheckCollisions(std::vector<BouncingThing> &collidableShapes) { //Check if 2 shapes are colliding using circle collision detection and resolve them
	int amountOfShapes = collidableShapes.size();
	for (int i = 0; i < amountOfShapes; i++){
		BouncingThing &cB = collidableShapes[i];
		for (int j = i + 1; j < amountOfShapes; j++){
			if (cB.isCollidingWith(collidableShapes[j])){
				cB.resolveCollisionWith(collidableShapes[j]);
				break;
			}
		}
	}
}

bool CheckIfColliding(BouncingThing& bT, std::vector<BouncingThing> &collidableShapes) { //If the area to spawn the shape is clear of other shapes
	bool colliding = false;
	int amountOfShapes = collidableShapes.size();
	for (int i = 0; i < amountOfShapes; i++){
		if (bT.isCollidingWith(collidableShapes[i])){
			bT.resolveCollisionWith(collidableShapes[i]);
			break;
		}
	}
	return colliding;
}



std::vector<sf::CircleShape> testCentres;


void AddShapes(const sf::String& shapeName, int amountToAdd, std::vector<BouncingThing> & collidables){

	const int MinRadius = 10; //min
	const int MaxRadius = 25; //max
	const float Speed = 200;
	const int ScreenWidth = 800;
	const int ScreenHeight = 600;

	for (int i = 0; i < amountToAdd; i++) {
		float radius = 20.f;// MinRadius + (rand() % (MaxRadius - MinRadius + 1));
		float xPos = 3 * MaxRadius + (rand() % (ScreenWidth - (6 * MaxRadius)));
		float yPos = 3 * MaxRadius + (rand() % (ScreenHeight - (6 * MaxRadius)));
		int dir = 1 - ((rand() % 2) * 2); //dir = 1 or -1
		float rndRotationSpeed = 0.f;// (float)(0.05 + ((rand() % 21) / 100));

		BouncingThing bT(radius, Speed, ScreenWidth, ScreenHeight, sf::Vector2f(radius*3, yPos), rndRotationSpeed * dir, 4);
		while (CheckIfColliding(bT, collidables));
		if (shapeName == "Square"){
			collidables.push_back(bT);
			std::cout << bT.getPosition().x << " ," << bT.getPosition().y << std::endl;
			testCentres.push_back(sf::CircleShape(radius/2.f));
			testCentres.back().setFillColor(sf::Color::Green);
			testCentres.back().setOrigin(radius / 2.f, radius / 2.f);
			testCentres.back().setPosition(collidables.back().getPosition());
			
		}
		else if (shapeName == "Triangle"){
			collidables.push_back(BouncingThing(radius, Speed, ScreenWidth, ScreenHeight,
				sf::Vector2f(xPos, yPos), rndRotationSpeed * dir, 3));
		}
	}
}
int main()
{
	// Create the main window 
	sf::RenderWindow window(sf::VideoMode(800, 600, 32), "Bouncing Things");

	//load a font
	sf::Font font;
	font.loadFromFile("C:\\Windows\\Fonts\\GARA.TTF");

	//create a formatted text string
	sf::Text dotProductCountText;
	dotProductCountText.setFont(font);
	dotProductCountText.setStyle(sf::Text::Regular);
	dotProductCountText.setPosition(20, 40);
	dotProductCountText.setCharacterSize(32);

	sf::Text fpsText;
	fpsText.setFont(font);
	fpsText.setStyle(sf::Text::Regular);
	fpsText.setPosition(20, 10);
	fpsText.setCharacterSize(32);

	//stats
	sf::Clock clock, frameTimer;
	int fps = 0;
	float frames = 0;
	clock.restart();
	frameTimer.restart();

	const int AmountOfSquares = 5;
	const int AmountOfTriangles = 0;
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
			fps = frames * (1/0.25);
			frames = 0;
			clock.restart();
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
		}
		float dt = frameTimer.getElapsedTime().asSeconds();
		frameTimer.restart();

		for (int i = 0; i < amountOfShapes; i++) {
			collidableShapes[i].Update(dt);
			testCentres[i].setPosition(collidableShapes[i].getPosition());
		}
		CheckCollisions(collidableShapes);
		fpsText.setString("Fps: " + std::to_string(fps));
	
		//prepare frame
		window.clear();

		//draw frame items
		window.draw(dotProductCountText);
		window.draw(fpsText);
		for (int i = 0; i < amountOfShapes; i++) {
			window.draw(collidableShapes[i].getShape());
			window.draw(testCentres[i]);
		}

		// Finally, display rendered frame on screen 
		window.display();
	} //loop back for next frame

	return EXIT_SUCCESS;
}