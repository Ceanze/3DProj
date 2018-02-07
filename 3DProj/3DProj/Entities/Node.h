#ifndef NODE_H
#define NODE_H

#include <glm.hpp>
#include <vector>

#include "../Utils/Transform.h"

class ShaderProgram;
class Display;

class Node
{
public:
	Node();
	virtual ~Node();

	void addChild(Node* child);
	void setParent(Node* parent);

	void init();
	void update(float dt);
	void input(Display* display);
	void render(ShaderProgram* shadowShader = nullptr);

	Node* getParent();
	std::vector<Node*>& getChildren();

	Transform& getWorldTransform();
	Transform& getLocalTransform();
	Transform& getChainTransform();

private:
	virtual void selfUpdate(float dt) = 0;
	virtual void selfRender(ShaderProgram* shadowShader) = 0;
	virtual void selfInput(Display* display) = 0;
	virtual void selfInit() = 0;

	Node* parent;
	std::vector<Node*> children;
	
	Transform worldTransform;
	Transform localTransform;
	Transform chainTransform;
};

#endif
