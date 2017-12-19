#include "StateLoader.h"

#include <fstream>
#include <sstream>

StateLoader::StateLoader(const std::string & fileName, Display * ptr)
{
	this->fileName = fileName;
	this->displayPtr = ptr;
}

StateLoader::~StateLoader()
{
	for (int i = 0; i < this->nrOfStates; i++)
		delete states[i];

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
					ss << line;
					int nrOf;
					std::string objFile;

					ss >> nrOf;

					for (int i = 0; i < nrOf; i++)
					{
						std::getline(file, line);
						ss << line;
						ss >> objFile;
						Mesh* temp = new Mesh();
						this->objLoader.load(temp, OBJPATH + objFile);
						this->meshes.push_back(temp);
					}

					std::getline(file, line);
					ss << line;
					ss >> nrOf;

					for (int i = 0; i < nrOf; i++)
					{
						
						

						std::getline(file, line);
						ss << line;
						ss >> selection >> pos.x >> pos.y >> pos.z >> scale >> dir.x >> dir.y >> dir.z;

						Model* temp = new Model(this->meshes[selection], pos, dir);
						this->models.push_back(temp);
					}

					std::getline(file, line);
					ss << line;
					ss >> nrOf;

					for (int i = 0; i < nrOf; i++)
					{
						bool isDynamic;
						std::getline(file, line);
						ss << line;
						unsigned nrOfModels = 0;
						ss >> nrOfModels;

						ss >> pos.x >> pos.y >> pos.z >> scale >> dir.x >> dir.y >> dir.z >> isDynamic;
						ss >> selection;
						Entity* temp = new Entity(*this->models[selection], pos, dir, isDynamic);
						for (int j = 0; j < nrOfModels; j++)
						{
							ss >> selection;
							temp->addModel(*this->models[selection]);
						}	
						this->entities.push_back[temp];
					}


				}
			}
			this->deleteModels();
		}

	}
}

void StateLoader::deleteModels()
{
	for (int i = 0; i < this->models.size(); i++)
		delete this->models[i];
}
