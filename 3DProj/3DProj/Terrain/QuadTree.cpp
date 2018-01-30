#include "QuadTree.h"


QuadTree::QuadTree(const unsigned & recursionLevel)
{
	this->hasChildren = false;
	if (recursionLevel > 0)
	{
		this->hasChildren = true;

		this->children = new QuadTree*[4];

		for (int i = 0; i < CHILDREN_AMOUNT; i++)
			this->children[i] = new QuadTree(recursionLevel - 1);
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

void QuadTree::addTriangleToChild(const unsigned & child, const Triangle& triangle)
{
	this->children[child]->triangles.push_back(triangle);
}

void QuadTree::render()
{
	for (int i = 0; i < CHILDREN_AMOUNT && this->hasChildren == true; i++)
		this->children[i]->render();

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
