// Despa Diana Alexandra 331CA

#pragma once
#include "MyVertexFormat.h"
#include <vector>

// Obiect 3D definit prin multimea de vertecsi si multimea de indecsi.
class Object
{
public:
	std::vector<MyVertexFormat> vertices;
	std::vector<unsigned int> indices;

	Object();
	Object(std::vector<MyVertexFormat> vertices, std::vector<unsigned int> indices);
	~Object();
};

