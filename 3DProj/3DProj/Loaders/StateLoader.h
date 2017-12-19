#ifndef STATELOADER_H
#define STATELOADER_H

#define OBJPATH "../Models/"

#define STATEFILEPATH ""

#include "..\Core\Display.h"
#include "ObjLoader.h"
#include "..\Core\States\GameState.h"
 
#include <string>
#include <vector>

class StateLoader
{
public:
	StateLoader(const std::string& fileName, Display * ptr);
	~StateLoader();

	std::vector<Entity*>* getEntities() { return &this->entities; };
	std::vector<Model*>* getModels() { return &this->models; };
	std::vector<Mesh*>* getMeshes() { return &this->meshes; };

	void readFile();

private:
	std::vector<Entity*> entities;
	std::vector<Model*> models;
	std::vector<Mesh*> meshes;
	ObjLoader objLoader;

	std::string fileName;

	void deleteModels();

private:
	Display * displayPtr;
	State** states;
	int nrOfStates;
};

#endif
