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

void Error::print(const std::string & str)
{
	print(str, CONSOLE_COLOR::WHITE);
}

void Error::print(const std::string & description, const std::string & str)
{
	print(description, str, CONSOLE_COLOR::WHITE);
}

void Error::print(const std::string & str, CONSOLE_COLOR color)
{
	print("", str, color);
}

void Error::print(const std::string & description, const std::string & str, CONSOLE_COLOR color)
{
	print(description, "", str, color);
}

void Error::print(const std::string & description1, const std::string & description2, const std::string & str, CONSOLE_COLOR color)
{
	// Tell the user how to stop
	SetConsoleTextAttribute(hstdout, color);
	const std::string s = description1 + description2;
	if (s.size() == 0)
	{
		std::cout << str.c_str() << std::endl;
	}
	else
	{
		std::cout << description1.c_str() << description2.c_str() << "\t" << str.c_str() << std::endl;
	}
	FlushConsoleInputBuffer(hstdin);

	// Keep users happy
	SetConsoleTextAttribute(hstdout, csbi.wAttributes);
}

void Error::printError(const std::string & str)
{
	printError("", str);

}

void Error::printError(const std::string & description, const std::string & str)
{
	print("[Error] ", description, str, CONSOLE_COLOR::RED);
}

void Error::printWarning(const std::string & str)
{
	printWarning("", str);
}

void Error::printWarning(const std::string & description, const std::string & str)
{
	print("[Warning] ", description, str, CONSOLE_COLOR::YELLOW);
}
