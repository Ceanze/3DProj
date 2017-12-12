#include "StateManager.h"



StateManager::StateManager()
{


}


StateManager::~StateManager()
{

}

State * StateManager::peek()
{
	return this->stateStack.top();
}

State * StateManager::pop()
{
	State* ret = this->stateStack.top();
	this->stateStack.pop();

	return ret;
}

void StateManager::push(State * state)
{
	this->stateStack.push(state);
}

unsigned int StateManager::size() const
{
	return this->stateStack.size();
}
