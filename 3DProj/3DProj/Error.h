#ifndef ERROR_H
#define ERROR_H

#include <iostream>
#include <Windows.h>

class Error
{
public:
	enum CONSOLE_COLOR
	{
		GREEN = 10,
		BLUE = 11,
		RED,
		PURPLE,
		YELLOW,
		WHITE
	};

	static void init();

	static void print(const std::string &str, CONSOLE_COLOR color);
	static void printError(const std::string &str);
	static void printWarning(const std::string &str);

private:
	static HANDLE hstdin, hstdout;
	static CONSOLE_SCREEN_BUFFER_INFO csbi;
};

#endif
