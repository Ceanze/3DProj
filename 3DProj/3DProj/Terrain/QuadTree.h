#ifndef QUADTREE_H
#define QUADTREE_H

#include "glew.h"
#include "glm.hpp"

#include <vector>

#define CHILDREN_AMOUNT 4

struct Triangle
{
	GLuint p1, p2, p3;
};

class QuadTree
{
public:
	QuadTree(const unsigned& recursionLevel, const glm::vec2 corners[4]);
	~QuadTree();

	QuadTree** children;
	bool hasChildren;

	void addTriangle(const glm::vec2 & pos, const Triangle& triangle);
	void render();

	void addEbo();

private:
	std::vector<Triangle> triangles;
	GLuint ebo;

	glm::vec2 corners[4];
	float quadSize;

};

#endif
