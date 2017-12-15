#include "Material.h"

Material::Material()
{
	this->ka = { 0.2f, 0.2f, 0.2f };
	this->kd = { 0.6f, 0.6f, 0.6f };
	this->ks = { 1.0f, 1.0f, 1.0f };
	this->ns = 90.0f;
}

Material::~Material()
{
}
