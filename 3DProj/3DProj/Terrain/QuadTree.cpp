#include "QuadTree.h"


QuadTree::QuadTree(const unsigned & recursionLevel, const glm::vec2 corners[4])
{
	this->hasChildren = false;

	for (int i = 0; i < 4; i++)
		this->corners[i] = corners[i];

	if (recursionLevel > 0)
	{
		this->hasChildren = true;

		this->children = new QuadTree*[4];

		this->quadSize = glm::length((corners[1] - corners[0]));

		glm::vec2 v1 = { 0, 0 }, v2 = { 0, 0 };
		v1 = (corners[1] - corners[0]) / 2.0f;
		v2 = (corners[2] - corners[0]) / 2.0f;
		glm::vec2 childCorners[4] = { corners[0], corners[0] + v1,  corners[0] + v2, corners[0] + v1 + v2 };
		
		this->children[0] = new QuadTree(recursionLevel - 1, childCorners);

		childCorners[0] = corners[0] + v1;
		childCorners[1] = corners[1];
		childCorners[2] = corners[0] + v1 + v2;
		childCorners[3] = corners[1] + v2;

		this->children[1] = new QuadTree(recursionLevel - 1, childCorners);

		childCorners[0] = corners[0] + v2;
		childCorners[1] = corners[0] + v1 + v2;
		childCorners[2] = corners[2];
		childCorners[3] = corners[2] + v1;

		this->children[2] = new QuadTree(recursionLevel - 1, childCorners);

		childCorners[0] = corners[0] + v1 + v2;
		childCorners[1] = corners[1] + v2;
		childCorners[2] = corners[2] + v1;
		childCorners[3] = corners[3];

		this->children[3] = new QuadTree(recursionLevel - 1, childCorners);
	}

	
}

QuadTree::~QuadTree()
{
	if (this->hasChildren)
	{
		for (int i = 0; i < CHILDREN_AMOUNT; i++)
			delete this->children[i];
		delete[] this->children;
	}
	
}

void QuadTree::addTriangle(const glm::vec2 & pos, const Triangle& triangle)
{
	float childQuadSize = this->quadSize / 2.0f;

	if (pos.x <= childQuadSize && pos.y <= childQuadSize)
	{
		this->children[0]->triangles.push_back(triangle);;
	}
	else if (pos.x < childQuadSize && pos.y > childQuadSize)
	{
		this->children[1]->triangles.push_back(triangle);
	}
	else if (pos.x > childQuadSize && pos.y < childQuadSize)
	{
		this->children[2]->triangles.push_back(triangle);
	}
	else
	{
		this->children[3]->triangles.push_back(triangle);
	}
}

void QuadTree::render()
{
	/*for (int i = 0; i < CHILDREN_AMOUNT && this->hasChildren == true; i++)
		this->children[i]->render();*/
	if(this->hasChildren)
		this->children[0]->render();

	if (this->triangles.size() > 0)
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->ebo);
		glDrawElements(GL_TRIANGLES, this->triangles.size() * 3, GL_UNSIGNED_INT, 0);
	}

}

void QuadTree::addEbo()
{
	if (this->triangles.size() > 0)
	{
		glGenBuffers(1, &this->ebo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*this->triangles.size() * 3, &this->triangles[0], GL_STATIC_DRAW);
	}

	if (this->hasChildren)
	{
		for (int i = 0; i < CHILDREN_AMOUNT; i++)
			this->children[i]->addEbo();
	}

}