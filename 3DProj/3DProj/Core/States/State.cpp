#include "State.h"

Display* State::displayPtr = nullptr;

State::State()
{
}

State::State(Display * ptr)
{
	this->displayPtr = ptr;
}

State::~State()
{
}
