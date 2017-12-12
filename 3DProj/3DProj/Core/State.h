#ifndef STATE_H
#define STATE_H

class State
{
public:
	State();
	virtual ~State();

	virtual void init() = 0;
};

#endif
