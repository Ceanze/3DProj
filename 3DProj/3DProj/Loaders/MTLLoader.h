#ifndef MTL_LOADER_H
#define MTL_LOADER_H

#include <string>
#include <map>

#include "../Entities/Models/Mesh.h"
#include "../Entities/Models/Material.h"

class MTLLoader
{
public:
	MTLLoader();

	std::map<std::string, Material*> load(const std::string & name, bool useNormalMap);

private:

};

#endif
