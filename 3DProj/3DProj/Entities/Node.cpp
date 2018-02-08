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

void Node::init()
{
	selfInit();

	for (Node* node : this->children)
		node->init();
}

void Node::update(float dt)
{
	selfUpdate(dt);

	for (Node* node : this->children)
		node->update(dt);

	if (this->parent != nullptr)
		this->chainTransform.setMatrix(this->parent->getChainTransform().getMatrix()*this->worldTransform.getMatrix());
	else
		this->chainTransform.setMatrix(this->worldTransform.getMatrix());
}

void Node::input(Display * display)
{
	this->selfInput(display);

	for (Node* node : this->children)
		node->input(display);
	
}

void Node::render(ShaderProgram* shadowShader)
{
	selfRender(shadowShader);

	for (Node* node : this->children)
		node->render(shadowShader);
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

Transform & Node::getChainTransform()
{
	return this->chainTransform;
}

