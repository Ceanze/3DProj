#include <iostream>
#include <Windows.h>
#include <crtdbg.h>

#include "Core\EngineCore.h"

#include "Error.h"

int main(void)
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	Error::init();

	EngineCore engine;

	engine.init();

	return 0;
}