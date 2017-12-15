#ifndef STATELOADER_H
#define STATELOADER_H

#define STATEFILEPATH ""


#include "Loaders\ObjLoader.h"
#include "Core\States\GameState.h"
 
#include <string>
#include <vector>

class StateLoader
{
public:
	StateLoader(const std::string& fileName);
	~StateLoader();

	void readFile();

private:
	std::vector<Model> models;
	ObjLoader objLoader;

	std::string fileName;
};

#endif
