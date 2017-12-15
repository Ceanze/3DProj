#include "StateLoader.h"

#include <fstream>
#include <sstream>

StateLoader::StateLoader(const std::string & fileName)
{
	this->fileName = fileName;
}

StateLoader::~StateLoader()
{
}

void StateLoader::readFile()
{
	std::ifstream file(this->fileName);

	if (file.is_open())
	{
		std::string line;

		while (std::getline(file, line))
		{
			std::stringstream ss(line);
			if (line[0] != '#') // If it is not a comment
			{
				std::string c;
				ss >> c;
		}
	}

}
