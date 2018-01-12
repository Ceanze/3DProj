#ifndef NODE_H
#define NODE_H

#include <glm.hpp>
#include <vector>

#include "../Utils/Transform.h"

class Node
{
public:
	Node();
	virtual ~Node();

	void addChild(Node* child);
	void setParent(Node* parent);

	void update(float dt);
	void render();

	Node* getParent();
	std::vector<Node*>& getChildren();

	Transform& getWorldTransform();
	Transform& getLocalTransform();

private:
	virtual void selfUpdate(float dt) = 0;
	virtual void selfRender() = 0;

	Node* parent;
	std::vector<Node*> children;
	
	Transform worldTransform;
	Transform localTransform;
};

#endif
