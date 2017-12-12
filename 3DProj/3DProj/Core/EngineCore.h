#ifndef ENGINECORE_H
#define ENGINECORE_H

#include "Display.h"
#include "Config.h"

class EngineCore
{
public:
	EngineCore();
	~EngineCore();

	void init();

private:
	void update(const float& dt);
	void render();

	Display display;
};

#endif // !ENGINECORE_H
