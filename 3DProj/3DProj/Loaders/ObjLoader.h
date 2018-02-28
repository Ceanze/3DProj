#ifndef OBJ_LOADER_H
#define OBJ_LOADER_H

#include "../Entities/Models/Mesh.h"
#include "../Core/Config.h"

class ObjLoader
{
public:
	void load(std::vector<Mesh*>& meshes, const std::string& name, unsigned int flags = 0);

private:
	void calculateNormal(Mesh* mesh, unsigned int triangle[3]) const;
	void calculateTangent(Mesh * mesh, unsigned int triangle[3]);
	void addMaterial(Material** material, std::vector<Material*>& outMaterials);
};

#endif
