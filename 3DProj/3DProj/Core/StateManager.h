#ifndef STATEMANAGER_H
#define STATEMANAGER_H

#include <stack>

#include "State.h"

class StateManager
{
public:
	StateManager();
	~StateManager();

	State* peek();
	//Remember to catch pointer
	State* pop();

	void push(State* state);
	
	const unsigned int size() const;

private:
	std::stack<State*> stateStack;

};

#endif
