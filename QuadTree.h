#ifndef QUADTREE_H
#define QUADTREE_H

#include "SFML/Graphics.hpp" 
#include "BouncingThing.h"

class QuadTree : public sf::Drawable{
public:
	QuadTree(int, sf::FloatRect);
	void clear();
	void insert(BouncingThing*);
	std::vector<BouncingThing*> retrieve(std::vector<BouncingThing*>&, BouncingThing*);
	virtual void draw(sf::RenderTarget& target, sf::RenderStates state) const;

private:
	void split();
	int getIndex(sf::FloatRect);
	sf::RectangleShape rectShape;

	const int MAX_OBJECTS = 5;
	const int MAX_LEVELS = 4;
	int m_level;
	std::vector<BouncingThing*> m_objects;
	std::vector<QuadTree> m_nodes;
	sf::FloatRect m_bounds;
};

#endif