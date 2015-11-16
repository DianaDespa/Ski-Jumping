// Despa Diana Alexandra 331CA

#pragma once
#include "..\dependente\glm\glm.hpp"
#include "..\dependente\glew\glew.h"
#include "..\dependente\glm\gtc\type_ptr.hpp"
#include "..\dependente\glm\gtc\matrix_transform.hpp"
#include "MyVertexFormat.h"
#include <vector>

// Clasa ce reprezinta quad-ul.
class Quad
{
private:
	unsigned int vao, vbo, ibo;	// Geometrie quad.
	unsigned int size, num_indices;
	glm::mat4 model_matrix;
	std::vector<std::vector<bool>> occupied; // Matrice care indica care patrate din quad sunt ocupate de personaje.

	void createGeometry();		// Creeaza geometria.

public:
	Quad(unsigned int size);
	~Quad();					// Distruge geometria.

	void drawGeometry(unsigned int gl_program_shader);	// Deseneaza quad.
	unsigned int getSize();								// Intoarce dimensiunea quad-ului.
	void setOccupied(int i, int j);						// Seteaza ca ocupat patratul de la (i, j).
	bool isOccupied(int i, int j);						// Verifica daca este ocupat patratul de la (i, j).
	std::pair<int, int> getFirstFree();		// Intoarce coordonatele primului patrat liber de pe quad, din partea de jos.
};

