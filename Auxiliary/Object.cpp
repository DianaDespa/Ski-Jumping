// Despa Diana Alexandra 331CA

#include "Object.h"


Object::Object()
{
}

Object::Object(std::vector<MyVertexFormat> vertices, std::vector<unsigned int> indices)
{
	this->vertices = vertices;
	this->indices = indices;
}

Object::~Object()
{
}
