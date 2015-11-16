// Despa Diana Alexandra 331CA

#pragma once
#include "Object.h"
#include "CharacterType.h"

// Clasa ce incarca toate tipurile de obiecte si le stocheaza in vectorul objects.
class ObjectBuilder
{
private:
	std::vector<Object*> objects;

public:
	ObjectBuilder();
	~ObjectBuilder();

	// Intoarce un anumit tip de obiect.
	void get(CharacterType type, std::vector<MyVertexFormat> &vertices, std::vector<unsigned int> &indices);
};

