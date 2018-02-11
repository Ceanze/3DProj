#include "QuadTree.h"
#include "../Error.h"

QuadTree::QuadTree(const unsigned & recursionLevel, glm::vec3 corners[4], const float& height)
{
	this->hasChildren = false;
	this->inFrustum = false;

	for (int i = 0; i < 4; i++)
	{	
		this->box.addPoint({ corners[i].x,  corners[i].y, corners[i].z});
		this->box.addPoint({ corners[i].x,  corners[i].y + height, corners[i].z });
	}
		

	if (recursionLevel > 0)
	{
		this->hasChildren = true;

		this->children = new QuadTree*[4];

		this->quadSize = glm::length((corners[1] - corners[0]));

		glm::vec3 v1 = { 0, 0, 0 }, v2 = { 0, 0, 0 };
		v1 = (corners[1] - corners[0]) / 2.0f;
		v2 = (corners[2] - corners[0]) / 2.0f;
		glm::vec3 childCorners[4] = { corners[0], corners[0] + v1,  corners[0] + v2, corners[0] + v1 + v2 };
		
		this->children[0] = new QuadTree(recursionLevel - 1, childCorners, height);

		childCorners[0] = corners[0] + v1;
		childCorners[1] = corners[1];
		childCorners[2] = corners[0] + v1 + v2;
		childCorners[3] = corners[1] + v2;

		this->children[1] = new QuadTree(recursionLevel - 1, childCorners, height);

		childCorners[0] = corners[0] + v2;
		childCorners[1] = corners[0] + v1 + v2;
		childCorners[2] = corners[2];
		childCorners[3] = corners[2] + v1;

		this->children[2] = new QuadTree(recursionLevel - 1, childCorners, height);

		childCorners[0] = corners[0] + v1 + v2;
		childCorners[1] = corners[1] + v2;
		childCorners[2] = corners[2] + v1;
		childCorners[3] = corners[3];

		this->children[3] = new QuadTree(recursionLevel - 1, childCorners, height);
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

void QuadTree::init()
{
	if (this->triangles.size() > 0 && this->hasChildren)
	{
		for (int i = 0; i < this->triangles.size(); i++)
		{
			this->addTriangle(this->trianglePositions[i], this->triangles[i]);
		}
		this->triangles.clear();
		this->trianglePositions.clear();

		for (int i = 0; i < CHILDREN_AMOUNT; i++)
		{
			this->children[i]->init();
		}
	}
}

void QuadTree::addTriangleToRoot(const glm::vec2 & pos, const Triangle & triangle)
{
	this->triangles.push_back(triangle);
	this->trianglePositions.push_back(pos);
}

void QuadTree::addTriangle(const glm::vec2 & pos, const Triangle& triangle)
{
	float childQuadSize = this->quadSize / 2.0f;

	if (pos.x <= childQuadSize && pos.y <= childQuadSize)
	{
		this->children[0]->triangles.push_back(triangle);
		this->children[0]->trianglePositions.push_back(pos);
	}
	else if (pos.x < childQuadSize && pos.y > childQuadSize)
	{
		this->children[1]->triangles.push_back(triangle);
		this->children[1]->trianglePositions.push_back(pos);
	}
	else if (pos.x > childQuadSize && pos.y < childQuadSize)
	{
		this->children[2]->triangles.push_back(triangle);
		this->children[2]->trianglePositions.push_back(pos);
	}
	else
	{
		this->children[3]->triangles.push_back(triangle);
		this->children[3]->trianglePositions.push_back(pos);
	}
}

void QuadTree::render()
{
	for (int i = 0; i < CHILDREN_AMOUNT && this->hasChildren == true; i++)
		this->children[i]->render();


	if (this->triangles.size() > 0 && this->inFrustum && !this->hasChildren)
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->ebo);
		glDrawElements(GL_TRIANGLES, this->triangles.size() * 3, GL_UNSIGNED_INT, 0);
	}

}

bool QuadTree::statusFrustum(const Plane planes[6])
{
	bool result = false;

	for (int plane = 0; plane < 6; plane++)
	{
		bool in = false;
		for (int pointBox = 0; pointBox < 8 && !in; pointBox++)
		{
			if (planes[plane].distance(this->box.getPoint(pointBox)) > 0)
			{
				in = true;
			}
		}

		if (!in)
		{
			this->inFrustum = false;
			result = false;
			return false;
		}
		else
			result = true;
	}

	if(result)
	{
		this->inFrustum = true;

		if (this->hasChildren)
		{
			for (int i = 0; i < CHILDREN_AMOUNT; i++)
			{
				this->children[i]->statusFrustum(planes);
			}
		}
	}

	return result;
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

AABox QuadTree::getBox() const
{
	return this->box;
}

bool QuadTree::isInFrustum() const
{
	return this->inFrustum;
}
