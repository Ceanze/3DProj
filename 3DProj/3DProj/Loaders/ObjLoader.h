#ifndef OBJ_LOADER_H
#define OBJ_LOADER_H

#include "../Entities/Models/Mesh.h"
#include "../Core/Config.h"

class ObjLoader
{
public:
	void load(Mesh* mesh, const std::string& name, unsigned int flags = USE_NORMAL_MAP);

private:
	void calculateNormal(Mesh* mesh, unsigned char triangle[3]) const;
	void calculateTangent(Mesh * mesh, unsigned char triangle[3]);
};

#endif
