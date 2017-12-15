#ifndef OBJ_LOADER_H
#define OBJ_LOADER_H

#include "../Entities/Models/Mesh.h"

class ObjLoader
{
public:
	void load(Mesh* mesh, const std::string& name);

private:
	void calculateNormal(Mesh* mesh, unsigned char triangle[3]) const;
};

#endif
