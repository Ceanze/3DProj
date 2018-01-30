#ifndef QUADTREE_H
#define QUADTREE_H

#include "glew.h"

#include <vector>

#define CHILDREN_AMOUNT 4

struct Triangle
{
	GLuint p1, p2, p3;
};

class QuadTree
{
public:
	QuadTree(const unsigned& recursionLevel);
	~QuadTree();

	QuadTree** children;
	bool hasChildren;

	void addTriangleToChild(const unsigned& child, const Triangle& triangle);
	void render();

	void addEbo();

private:
	std::vector<Triangle> triangles;
	GLuint ebo;

};

#endif
