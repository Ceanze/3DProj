#ifndef STATE_H
#define STATE_H

#include "../Display.h"

class State
{
public:
	State();
	State(Display * ptr);
	virtual ~State();

	virtual void init() = 0;

	virtual void update(float dt) = 0;
	virtual void input() = 0;
	virtual void render() = 0;

private:
	static Display * displayPtr;

};

#endif
