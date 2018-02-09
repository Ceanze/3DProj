#include "ObjLoader.h"

#include <fstream>
#include <sstream>

#include <glm.hpp>
#include <gtc\type_ptr.hpp>

#include "MTLLoader.h"

#include "../Error.h"
#include "../Core/ResourceManager.h"

void ObjLoader::load(Mesh * mesh, const std::string & name, unsigned int flags)
{
	MTLLoader mtlLoader;
	std::vector<std::map<std::string, Material*>> materials;

	mesh->vertices.clear();
	mesh->indices.clear();
	
	std::vector<glm::vec3> temp_vertices;
	std::vector<glm::vec3> temp_normals;
	std::vector<glm::vec2> temp_uvs;

	std::vector<Material*> outMaterials;

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
					str.clear();
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
					if (flags & FLIP_UV_X)
						texCoords.x = 1.0f - texCoords.x;
					if (flags & FLIP_UV_Y)
						texCoords.y = 1.0f - texCoords.y;
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
							addMaterial(&mesh->material, outMaterials);
						}
					}
					str.clear();
				}

				if (c == "f") // Face (triangle)
				{
					unsigned int triangle[3];
					ss >> str;
					for (uint8_t i = 0; !str.empty(); ss >> str, i++) // Loop until there are no vertices left in this face.
					{
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

						if (i == 0)
							triangle[0] = vertIndex;

						if (i > 1) // This runs for each triangle.
						{
							numv += 3;
							triangle[1] = vertIndex - 1;
							triangle[2] = vertIndex;

							mesh->indices.push_back(triangle[0]);
							mesh->indices.push_back(triangle[1]);
							mesh->indices.push_back(triangle[2]);

							if (!hasNormal)
								calculateNormal(mesh, triangle);
							if (flags & USE_NORMAL_MAP)
								calculateTangent(mesh, triangle);
						}

						str.clear();
					}
				}
			}
		}

		if (materials.empty())
		{
			mesh->material = new Material(flags & USE_NORMAL_MAP);
			addMaterial(&mesh->material, outMaterials);
		}

		ResourceManager::addMaterials(outMaterials);

		Error::print("[Mesh detail]:", "[" + std::string(MODEL_PATH) + name + "] num triangles: " + std::to_string(numv));
		file.close();
	}
	else
	{
		Error::printError("Could not open obj file: " + name);
	}
}

void ObjLoader::load(std::vector<Mesh*>& meshes, const std::string & name, unsigned int flags)
{
	MTLLoader mtlLoader;
	std::vector<std::map<std::string, Material*>> materials;

	std::vector<glm::vec3> temp_vertices;
	std::vector<glm::vec3> temp_normals;
	std::vector<glm::vec2> temp_uvs;

	std::vector<Material*> outMaterials;

	int meshIndex = -1;

	int numv = 0;

	std::ifstream file(MODEL_PATH + name);
	if (file.is_open())
	{
		std::string line;

		std::string str;
		unsigned int indexGroup[3] = { 0, 0, 0 };
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
					mtlName = mtlName.substr(0, mtlName.find_last_of("/\\") + 1).append(str);
					materials.push_back(mtlLoader.load(mtlName, flags & USE_NORMAL_MAP));
					str.clear();
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
					if (flags & FLIP_UV_X)
						texCoords.x = 1.0f - texCoords.x;
					if (flags & FLIP_UV_Y)
						texCoords.y = 1.0f - texCoords.y;
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
					meshIndex++;
					meshes.push_back(new Mesh());

					ss >> str;
					std::map<std::string, Material*>::iterator it;
					const int numMtllib = materials.size();
					for (int i = 0; i < numMtllib; i++)
					{
						it = materials[i].find(str);
						if (it != materials[i].end())
						{
							meshes[meshIndex]->material = it->second;
							addMaterial(&meshes[meshIndex]->material, outMaterials);
						}
					}
					str.clear();
				}

				if (c == "f") // Face (triangle)
				{
					unsigned int triangle[3];
					ss >> str;
					for (uint8_t i = 0; !str.empty(); ss >> str, i++) // Loop until there are no vertices left in this face.
					{
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

						meshes[meshIndex]->vertices.push_back(vertex);
						int vertIndex = meshes[meshIndex]->vertices.size() - 1;

						if (i == 0)
							triangle[0] = vertIndex;

						if (i > 1) // This runs for each triangle.
						{
							numv += 3;
							triangle[1] = vertIndex - 1;
							triangle[2] = vertIndex;
							
							meshes[meshIndex]->indices.push_back(triangle[0]);
							meshes[meshIndex]->indices.push_back(triangle[1]);
							meshes[meshIndex]->indices.push_back(triangle[2]);
							
							if (!hasNormal)
								calculateNormal(meshes[meshIndex], triangle);
							if (flags & USE_NORMAL_MAP)
								calculateTangent(meshes[meshIndex], triangle);
						}

						str.clear();
					}
				}
			}
		}

		if (materials.empty())
		{
			for (int i = 0; i < meshIndex + 1; i++)
			{
				meshes[i]->material = new Material(flags & USE_NORMAL_MAP);
				addMaterial(&meshes[i]->material, outMaterials);
			}
		}
		else
		{
			for (int i = 0; i < meshIndex + 1; i++)
				if (meshes[i]->material == nullptr)
				{
					meshes[i]->material = new Material(flags & USE_NORMAL_MAP);
					addMaterial(&meshes[i]->material, outMaterials);
				}
		}

		ResourceManager::addMaterials(outMaterials);

		Error::print("[Mesh detail]:", "["+ std::string(MODEL_PATH) + name +"] num triangles: " + std::to_string(numv) + ", meshIndex: " + std::to_string(meshIndex));
		file.close();
	}
	else
	{
		Error::printError("Could not open obj file: " + name);
	}
}

void ObjLoader::calculateNormal(Mesh * mesh, unsigned int triangle[3]) const
{
	const glm::vec3 v1 = mesh->vertices[triangle[0]].position;
	const glm::vec3 v2 = mesh->vertices[triangle[1]].position;
	const glm::vec3 v3 = mesh->vertices[triangle[2]].position;
	const glm::vec3 normal = glm::normalize(glm::cross(v2 - v1, v3 - v1));

	mesh->vertices[triangle[0]].normal = normal;
	mesh->vertices[triangle[1]].normal = normal;
	mesh->vertices[triangle[2]].normal = normal;
}

void ObjLoader::calculateTangent(Mesh * mesh, unsigned int triangle[3])
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

void ObjLoader::addMaterial(Material ** material, std::vector<Material*>& outMaterials)
{
	bool hasMaterial = false;
	for (Material* m : outMaterials)
		if (m == *material)
			hasMaterial = true;
	if (!hasMaterial)
		outMaterials.push_back(*material);
}
