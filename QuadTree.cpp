#include "QuadTree.h"
//http://gamedevelopment.tutsplus.com/tutorials/quick-tip-use-quadtrees-to-detect-likely-collisions-in-2d-space--gamedev-374

QuadTree::QuadTree(int level, sf::FloatRect bounds) :
m_level(level),
m_bounds(bounds){
	rectShape = sf::RectangleShape(sf::Vector2f(bounds.width, bounds.height));
	rectShape.setPosition(bounds.left, bounds.top);
	rectShape.setFillColor(sf::Color::Transparent);
	rectShape.setOutlineColor(sf::Color::Green);
	rectShape.setOutlineThickness(1.f);
}

void QuadTree::clear(){
	m_objects.clear();
	m_objects.shrink_to_fit();
	int nodeCount = m_nodes.size();
	for (int i = 0; i < nodeCount; i++){
		m_nodes.back().clear();
		m_nodes.pop_back();
	}
}

void QuadTree::split(){
	float halfWidth = m_bounds.width / 2.f;
	float halfheight = m_bounds.height / 2.f;
	float x = m_bounds.left;
	float y = m_bounds.top;

	m_nodes.push_back(QuadTree(m_level + 1, sf::FloatRect(x, y, halfWidth, halfheight))); //top left
	m_nodes.push_back(QuadTree(m_level + 1, sf::FloatRect(x + halfWidth, y, halfWidth, halfheight))); //top right
	m_nodes.push_back(QuadTree(m_level + 1, sf::FloatRect(x + halfWidth, y + halfheight, halfWidth, halfheight))); //bottom right
	m_nodes.push_back(QuadTree(m_level + 1, sf::FloatRect(x, y + halfheight, halfWidth, halfheight))); //bottom left
}

int QuadTree::getIndex(sf::FloatRect rect){
	int index = -1;

	bool top = (rect.top + rect.height < (m_bounds.top + m_bounds.height / 2.f));
	bool bottom = (rect.top > (m_bounds.top + m_bounds.height / 2.f));

	if (rect.left + rect.width < (m_bounds.left + m_bounds.width / 2.f)){ //left half
		if (top)
			index = 0;
		else if (bottom)
			index = 3;
	}
	else if (rect.left >(m_bounds.left + m_bounds.width / 2.f)){ //right half
		if (top)
			index = 1;
		else if (bottom)
			index = 2;
	}

	return index;
}

void QuadTree::insert(BouncingThing* object){
	sf::FloatRect rect = object->getRect();
	if (m_nodes.size() != 0){//there is nodes to put the object into
		int index = getIndex(rect);
		if (index != -1){ //index doesnt indicate an overlapping object
			m_nodes[index].insert(object);
			return;
		}
	}

	m_objects.push_back(object); //keep adding objects to the current node

	if (m_objects.size() >= MAX_OBJECTS && m_level < MAX_LEVELS){ //too many objects and can still go down another level
		if (m_nodes.size() == 0)//there are no nodes
			split();

		int i = 0;  
		while (i < m_objects.size()){ //for all the objects
			int index = getIndex(m_objects[i]->getRect()); //get which node it should be placed in
			if (index != -1){
				m_nodes[index].insert(m_objects[i]);
				m_objects.erase(m_objects.begin() + i);
			}
			else
				i++;
		}
	}
}

std::vector<BouncingThing*> QuadTree::retrieve(std::vector<BouncingThing*>& possibleColliders, BouncingThing* object){
	int index = getIndex(object->getRect());
	if (index != -1 && m_nodes.size() != 0) {
		m_nodes[index].retrieve(possibleColliders, object);
	}
	for (int i = 0; i < m_objects.size(); i++){
		if (m_objects[i] != object)
			possibleColliders.push_back(m_objects[i]);
	}

	return possibleColliders;
}


void QuadTree::draw(sf::RenderTarget& target, sf::RenderStates state) const{
	if (m_nodes.size() != 0){
		for (int i = 0; i < m_nodes.size(); i++){
			target.draw(m_nodes[i]);
		}
	}
	target.draw(rectShape);
}