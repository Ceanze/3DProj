#include "MTLLoader.h"

#include <fstream>
#include <sstream>

#include "../Core/Config.h"
#include "../Error.h"

MTLLoader::MTLLoader()
{
}

std::map<std::string, Material*> MTLLoader::load(const std::string & name, bool useNormalMap)
{
	std::map<std::string, Material*> materials;

	Material* material = nullptr;
	int lineNumber = 0;
	std::ifstream file(MODEL_PATH + name);
	if (file.is_open())
	{
		std::string line;

		std::string strTemp;
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
					material = new Material(useNormalMap);
					ss >> mtlName;
				}

				// Specular exponent
				if (c.compare("Ns") == 0 || c.compare("ns") == 0)
					ss >> material->ns;

				// Ambient color
				if (c.compare("Ka") == 0 || c.compare("ka") == 0)
					ss >> material->ka.x >> material->ka.y >> material->ka.z;

				// Diffuse color
				if (c.compare("Kd") == 0 || c.compare("kd") == 0)
					ss >> material->kd.x >> material->kd.y >> material->kd.z;

				// Specular color
				if (c.compare("Ks") == 0 || c.compare("ks") == 0)
					ss >> material->ks.x >> material->ks.y >> material->ks.z;

				// Texture (albedo map)
				if (c.compare("map_Kd") == 0 || c.compare("map_kd") == 0)
				{
					ss >> strTemp;
					while (!strTemp.empty())
					{
						if (strTemp[0] == '-')
						{
							// Options.
							Error::printError("Option for map_Kd in mtl file is not supported!");
						}
						else
						{
							// Load texture.
							std::string texName(name);
							texName = std::string(MODEL_PATH) + texName.substr(0, texName.find_last_of("/\\") + 1).append(strTemp);
							material->loadTexture(texName);
						}
						strTemp.clear();
						ss >> strTemp;
					}
				}

				// Normal map
				if (c.compare("map_bump") == 0 || c.compare("map_Bump") == 0)
				{
					ss >> strTemp;
					while (!strTemp.empty())
					{
						if (strTemp[0] == '-')
						{
							// Options.
							Error::printError("Option for map_bump in mtl file is not supported!");
						}
						else
						{
							// Load texture.
							std::string texName(name);
							texName = std::string(MODEL_PATH) + texName.substr(0, texName.find_last_of("/\\") + 1).append(strTemp);
							material->loadNormalMap(texName);
						}
						strTemp.clear();
						ss >> strTemp;
					}
				}
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
		materials[DEFAULT_MATERIAL_NAME] = new Material(useNormalMap);
	}

	return materials;
}
