#include "ObjLoader.h"

#include <fstream>
#include <sstream>

#include <glm.hpp>
#include <gtc\type_ptr.hpp>

#include "MTLLoader.h"

#include "../Error.h"

void ObjLoader::load(Mesh * mesh, const std::string & name, unsigned int flags)
{
	MTLLoader mtlLoader;
	std::vector<std::map<std::string, Material*>> materials;

	mesh->vertices.clear();
	mesh->indices.clear();
	
	std::vector<glm::vec3> temp_vertices;
	std::vector<glm::vec3> temp_normals;
	std::vector<glm::vec2> temp_uvs;

	int numv = 0;

	std::ifstream file(MODEL_PATH+name);
	if (file.is_open())
	{
		std::string line;

		std::string str;
		unsigned int indexGroup[3] = {0, 0, 0};
		Mesh::Vertex vertex;
		bool hasNormal = false;
		bool hasUvs = false;
		while (std::getline(file, line))
		{
			std::stringstream ss(line);
			if (line[0] != '#') // If it is not a comment
			{
				std::string c;
				ss >> c;
				if (c == "mtllib")
				{
					ss >> str;
					std::string mtlName(name);
					mtlName = mtlName.substr(0, mtlName.find_last_of("/\\") +1).append(str);
					materials.push_back(mtlLoader.load(mtlName, flags & USE_NORMAL_MAP));
				}

				if (c == "v") // Vertex position
				{
					glm::vec3 pos;
					ss >> pos.x >> pos.y >> pos.z;
					temp_vertices.push_back(pos);
				}

				if (c == "vt") // Vertex uvs
				{
					glm::vec2 texCoords;
					ss >> texCoords.x >> texCoords.y;
					temp_uvs.push_back(texCoords);
					hasUvs = true;
				}

				if (c == "vn") // Vertex normal
				{
					glm::vec3 normal;
					ss >> normal.x >> normal.y >> normal.z;
					temp_normals.push_back(normal);
					hasNormal = true;
				}

				if (c == "usemtl") // Use material
				{
					ss >> str;
					std::map<std::string, Material*>::iterator it;
					const int numMtllib = materials.size();
					for (int i = 0; i < numMtllib; i++)
					{
						it = materials[i].find(str);
						if (it != materials[i].end())
						{
							mesh->material = it->second;
						}
					}
				}

				if (c == "f") // Face (triangle)
				{
					unsigned char triangle[3];
					for (uint8_t i = 0; i < 3; i++)
					{
					numv++;
						ss >> str;
						// Get vertex data.
						if (!hasUvs && hasNormal)
							sscanf_s(str.c_str(), "%d//%d", &indexGroup[0], &indexGroup[2]);
						else if (!hasNormal && hasUvs)
							sscanf_s(str.c_str(), "%d/%d", &indexGroup[0], &indexGroup[1]);
						else if (!hasNormal && !hasUvs)
							sscanf_s(str.c_str(), "%d", &indexGroup[0]);
						else
							sscanf_s(str.c_str(), "%d/%d/%d", &indexGroup[0], &indexGroup[1], &indexGroup[2]);

						vertex.position = temp_vertices[indexGroup[0] - 1];

						if (hasNormal)
							vertex.normal = temp_normals[indexGroup[2] - 1];

						if (hasUvs)
							vertex.uvs = temp_uvs[indexGroup[1] - 1];
						else
							vertex.uvs = { 0.0f, 0.0f };


						mesh->vertices.push_back(vertex);
						int vertIndex = mesh->vertices.size() - 1;
						mesh->indices.push_back(vertIndex);
						triangle[i] = vertIndex;
					}

					if (!hasNormal)
						calculateNormal(mesh, triangle);
					calculateTangent(mesh, triangle);
				}
			}
		}

		if (materials.empty())
			mesh->material = new Material(flags & USE_NORMAL_MAP);
		Error::printError("numv: " + std::to_string(numv));
		file.close();
	}
	else
	{
		Error::printError("Could not open obj file: " + name);
	}
}

void ObjLoader::calculateNormal(Mesh * mesh, unsigned char triangle[3]) const
{
	const glm::vec3 v1 = mesh->vertices[triangle[0]].position;
	const glm::vec3 v2 = mesh->vertices[triangle[1]].position;
	const glm::vec3 v3 = mesh->vertices[triangle[2]].position;
	const glm::vec3 normal = glm::normalize(glm::cross(v2 - v1, v3 - v1));

	mesh->vertices[triangle[0]].normal = normal;
	mesh->vertices[triangle[1]].normal = normal;
	mesh->vertices[triangle[2]].normal = normal;
}

void ObjLoader::calculateTangent(Mesh * mesh, unsigned char triangle[3])
{
	Mesh::Vertex& v0 = mesh->vertices[triangle[0]];
	Mesh::Vertex& v1 = mesh->vertices[triangle[1]];
	Mesh::Vertex& v2 = mesh->vertices[triangle[2]];
	glm::vec2 deltaUV1 = v1.uvs - v0.uvs;
	glm::vec2 deltaUV2 = v2.uvs - v0.uvs;
	glm::vec3 deltaPos1 = v1.position - v0.position;
	glm::vec3 deltaPos2 = v2.position - v0.position;
	/* Calculat tangent T from the equations:
	deltaPos1 = delatUV1.x*T + deltaUV1.y*B
	deltaPos2 = delatUV2.x*T + deltaUV2.y*B
	*/
	float r = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV1.y * deltaUV2.x);
	glm::vec3 tangent = (deltaPos1 * deltaUV2.y - deltaPos2 * deltaUV1.y)*r;

	// Make the tangent perpendicular to its corresponding normal.
	v0.tangent = glm::normalize(tangent - v0.normal*glm::dot(v0.normal, tangent));
	v1.tangent = glm::normalize(tangent - v1.normal*glm::dot(v1.normal, tangent));
	v2.tangent = glm::normalize(tangent - v2.normal*glm::dot(v2.normal, tangent));
}
