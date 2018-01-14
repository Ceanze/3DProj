#ifndef COMPONENT_H
#define COMPONENT_H

class Entity;
class Display;

class Component
{
public:
	Component();
	virtual ~Component();

	virtual void init() = 0;
	virtual void update(const float& dt) = 0;
	virtual void input(Display* display) = 0;

	void	setEntity(Entity * ptr);
	Entity* getEntity();

private:
	Entity* entity;
};

#endif
