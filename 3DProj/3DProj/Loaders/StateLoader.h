#ifndef STATELOADER_H
#define STATELOADER_H



#include "..\Core\Display.h"
#include "ObjLoader.h"
#include "..\Core\States\GameState.h"
 
#include <string>
#include <vector>

class StateLoader
{
public:
	StateLoader(const std::string& fileName, Display * ptr);
	StateLoader();
	~StateLoader();

	std::vector<Entity*>* getEntities() { return &this->entities; };
	std::vector<Mesh*>* getMeshes() { return &this->meshes; };

	void readFile();

private:
	std::vector<Entity*> entities;
	std::vector<Mesh*> meshes;
	ObjLoader objLoader;

	std::string fileName;

private:
	Display * displayPtr;
	State** states;
	int nrOfStates;
};

#endif
