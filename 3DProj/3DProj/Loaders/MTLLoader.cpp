#include "MTLLoader.h"

#include <fstream>
#include <sstream>

#include "../Core/Config.h"
#include "../Error.h"

MTLLoader::MTLLoader()
{
}

std::map<std::string, Material*> MTLLoader::load(const std::string & name)
{
	std::map<std::string, Material*> materials;

	Material* material = nullptr;
	int lineNumber = 0;
	std::ifstream file(MODEL_PATH + name);
	if (file.is_open())
	{
		std::string line;

		std::string mtlName;
		while (std::getline(file, line))
		{
			lineNumber++;
			std::stringstream ss(line);
			if (line[0] != '#') // If it is not a comment
			{
				std::string c;
				ss >> c;
				if (c.compare("newmtl") == 0) // New material
				{
					// If there is a material already, then add it to the list of materials. 
					if (material != nullptr)
						materials[mtlName] = material;

					// Construct the new material
					material = new Material();
					ss >> mtlName;
				}

				if (c.compare("Ns") == 0 || c.compare("ns") == 0)
					ss >> material->ns;

				if (c.compare("Ka") == 0 || c.compare("ka") == 0)
					ss >> material->ka.x >> material->ka.y >> material->ka.z;

				if (c.compare("Kd") == 0 || c.compare("kd") == 0)
					ss >> material->kd.x >> material->kd.y >> material->kd.z;

				if (c.compare("Ks") == 0 || c.compare("ks") == 0)
					ss >> material->ks.x >> material->ks.y >> material->ks.z;
			}
		}

		// Add the last material to the list of materials.
		if (material != nullptr)
			materials[mtlName] = material;

		file.close();
	}
	else
	{
		Error::printError("Could not open mtl file: " + name);
	}

	// If the file could not be found or if there where no materials in the file, set the material to the default material.
	if (materials.empty())
	{
		materials[DEFAULT_MATERIAL_NAME] = new Material();
	}

	return materials;
}
