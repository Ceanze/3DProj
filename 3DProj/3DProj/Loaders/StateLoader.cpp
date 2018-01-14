#include "StateLoader.h"

#include <fstream>
#include <sstream>

#include "../Error.h"

StateLoader::StateLoader(const std::string & fileName, Display * ptr)
{
	this->nrOfStates = 0;

	this->fileName = fileName;
	this->displayPtr = ptr;

	this->readFile();
}

StateLoader::StateLoader()
{
}

StateLoader::~StateLoader()
{
	for (int i = 0; i < this->nrOfStates; i++)
		delete states[i];
	if(this->nrOfStates > 0)
		delete[] states;

	for (int i = 0; i < this->entities.size(); i++)
		delete this->entities[i];
	for (int i = 0; i < this->meshes.size(); i++)
		delete this->meshes[i];

}

void StateLoader::readFile()
{
	std::ifstream file(this->fileName);
	int currentState = 0;

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
				
				if (c == "states")
				{
					int nrOfStates;
					ss >> nrOfStates;

					this->states = new State*[nrOfStates];

					for (int i = 0; i < nrOfStates; i++)
					{
						this->nrOfStates++;
						ss >> c;
						if (c == "gameState")
						{
							this->states[i] = new GameState(this->displayPtr);
						}
						else if (c == "menuState")
						{
							
						}
					}
				}

				if (c == "begin")
				{
					glm::vec3 pos, dir;
					float scale;
					unsigned selection;
					std::getline(file, line);
					int nrOf;
					std::string objFile;
					
					nrOf = std::stoi(line);
					//If we load in 0 meshes skip loading in the rest of the file
					if (nrOf > 0)
					{
						for (int i = 0; i < nrOf; i++)
						{
							std::getline(file, line);
							std::stringstream xx(line);
							xx >> objFile;
							Mesh* temp = new Mesh();
							this->objLoader.load(temp, objFile);
							this->meshes.push_back(temp);
						}

						std::getline(file, line);
						nrOf = std::stoi(line);

						//for (int i = 0; i < nrOf; i++)
						//{
						//	std::getline(file, line);
						//	std::stringstream xx(line);
						//	xx >> selection >> pos.x >> pos.y >> pos.z >> scale >> dir.x >> dir.y >> dir.z;

						///*	Model* temp = new Model(this->meshes[selection], pos, glm::normalize(dir), scale);
						//	this->models.push_back(temp);*/
						//}

						std::getline(file, line);
						nrOf = std::stoi(line);

						for (int i = 0; i < nrOf; i++)
						{
							bool isDynamic;
							std::getline(file, line);
							std::stringstream xx(line);
							unsigned nrOfModels = 0;
							xx >> nrOfModels;

							xx >> pos.x >> pos.y >> pos.z >> scale >> dir.x >> dir.y >> dir.z >> isDynamic;
							xx >> selection;
							//Entity* temp = new Entity(*this->models[selection], pos, glm::normalize(dir), isDynamic);
							for (int j = 0; j < nrOfModels - 1; j++)
							{
								xx >> selection;
								//temp->addModel(*this->models[selection]);
							}
							//this->entities.push_back(temp);
						}
					}
				}
			}
			
		}
	}
	else
	{
		Error::printError("no file found");
	}
}
