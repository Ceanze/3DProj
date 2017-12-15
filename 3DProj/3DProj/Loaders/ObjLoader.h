#ifndef OBJ_LOADER_H
#define OBJ_LOADER_H

#include "../Entities/Models/Mesh.h"

class ObjLoader
{
public:
	void load(Mesh* mesh, const std::string& name);

private:
	void calculateNormal(Mesh* mesh, unsigned char triangle[3]) const;
	void addVertex(const std::string &str, Mesh* mesh, const std::vector<glm::vec3> &temp_position, const std::vector<glm::vec3> &temp_normal, const std::vector<glm::vec2> &temp_uvs, Mesh::Vertex &vertex, unsigned char indexGroup[3]) const;
};

#endif
