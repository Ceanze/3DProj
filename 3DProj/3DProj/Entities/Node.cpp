#include "Node.h"

#include <gtc\matrix_transform.hpp>

Node::Node()
{
	this->parent = nullptr;
}

Node::~Node()
{
	for (unsigned i = 0; i < this->children.size(); i++)
	{
		delete this->children[i];
		this->children[i] = nullptr;
	}
}

void Node::addChild(Node * child)
{
	child->setParent(this);
	this->children.push_back(child);
}

void Node::setParent(Node * parent)
{
	this->parent = parent;
}

void Node::update(float dt)
{
	selfUpdate(dt);

	for (Node* node : this->children)
		node->update(dt);

	if (this->parent != nullptr)
		this->worldTransform.setWorldMatrix(this->parent->getWorldTransform().getWorldMatrix()*this->worldTransform.getLocalMatrix());
	else
		this->worldTransform.setWorldMatrix(this->worldTransform.getLocalMatrix());
}

void Node::render()
{
	selfRender();

	for (Node* node : this->children)
		node->render();
}

Node * Node::getParent()
{
	return this->parent;
}

std::vector<Node*>& Node::getChildren()
{
	return this->children;
}

Transform& Node::getWorldTransform()
{
	return this->worldTransform;
}

Transform& Node::getLocalTransform()
{
	return this->localTransform;
}

