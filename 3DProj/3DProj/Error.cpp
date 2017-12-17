#include "Error.h"

HANDLE Error::hstdin = (void*)0;
HANDLE Error::hstdout = (void*)0;
CONSOLE_SCREEN_BUFFER_INFO Error::csbi = {};

void Error::init()
{
	hstdin = GetStdHandle(STD_INPUT_HANDLE);
	hstdout = GetStdHandle(STD_OUTPUT_HANDLE);

	// Remember how things were when we started
	GetConsoleScreenBufferInfo(hstdout, &csbi);
}

void Error::print(const std::string & str, CONSOLE_COLOR color)
{
	// Tell the user how to stop
	SetConsoleTextAttribute(hstdout, color);
	std::cout << str.c_str() << std::endl;

	FlushConsoleInputBuffer(hstdin);

	// Keep users happy
	SetConsoleTextAttribute(hstdout, csbi.wAttributes);
}

void Error::printError(const std::string & str)
{
	// Tell the user how to stop
	SetConsoleTextAttribute(hstdout, CONSOLE_COLOR::RED);
	std::cout << "Error:\t\t" << str.c_str() << std::endl;

	FlushConsoleInputBuffer(hstdin);

	// Keep users happy
	SetConsoleTextAttribute(hstdout, csbi.wAttributes);
}

void Error::printWarning(const std::string & str)
{
	// Tell the user how to stop
	SetConsoleTextAttribute(hstdout, CONSOLE_COLOR::YELLOW);
	std::cout << "Warning:\t" << str.c_str() << std::endl;

	FlushConsoleInputBuffer(hstdin);

	// Keep users happy
	SetConsoleTextAttribute(hstdout, csbi.wAttributes);
}

