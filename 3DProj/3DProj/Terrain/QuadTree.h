#ifndef QUADTREE_H
#define QUADTREE_H

#include "glew.h"
#include "glm.hpp"
#include "../Core/Frustum Culling/Plane.h"
#include "../Core/Frustum Culling/AABox.h"

#include <vector>

#define CHILDREN_AMOUNT 4

struct Triangle
{
	GLuint p1, p2, p3;
};

class QuadTree
{
public:QuadTree(const unsigned& recursionLevel, glm::vec3 corners[4], const float& height);
	~QuadTree();

	QuadTree** children;
	bool hasChildren;

	void init();

	void addTriangleToRoot(const glm::vec2 pos, const Triangle triangle);
	void render();
	bool statusFrustum(const Plane planes[6]);

	void addEbo();

	AABox getBox() const;
	bool isInFrustum() const;
	void setChildrenInFrustum(bool state);

private:
	void addTriangle(const glm::vec2 & pos, const Triangle& triangle);
	std::vector<Triangle> triangles;
	std::vector<glm::vec2> trianglePositions;
	GLuint ebo;

	bool inFrustum;

	//glm::vec3 corners[4];

	AABox box;
	float quadSize;

};

#endif
