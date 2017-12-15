#ifndef GAMESTATE_H
#define GAMESTATE_H

#include "State.h"
#include "../../Entities/Entity.h"

class GameState : public State
{
public:
	GameState(Display * ptr);
	virtual ~GameState();

	void init();

	void update(float dt);
	void input();
	void render();

private:
	static Display * displayPtr;

};

#endif

