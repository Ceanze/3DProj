#include "Mesh.h"
#include <stdio.h>

#include <gtc\type_ptr.hpp>
#include <gtc\matrix_transform.hpp>

Mesh::Mesh()
{
}

Mesh::~Mesh()
{
	delete this->material;
}
