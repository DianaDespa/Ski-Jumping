// Despa Diana Alexandra 331CA

#pragma once
#include "..\dependente\glm\glm.hpp"
#include "..\dependente\glew\glew.h"
#include "..\dependente\glm\gtc\type_ptr.hpp"
#include "..\dependente\glm\gtc\matrix_transform.hpp"
#include "MyVertexFormat.h"
#include "Util.h"

#define _USE_MATH_DEFINES
#include <math.h>

// Tipuri de trambulina - principala sau secundara.
enum SlopeType {
	ACTIVE,
	PASSIVE
};

// Clasa ce reprezinta o trambulina.
class Slope
{
private:
	unsigned int vao, vbo, ibo, num_indices;	// Geometrie trambulina.
	std::vector<MyVertexFormat> vertices;
	std::vector<unsigned int> indices;
	glm::mat4 model_matrix;
	glm::vec3 color;
	SlopeType type;
	int angle;
	int width;
	glm::vec3 top;

	void createGeometry();						// Creeaza geometria.
	void addVertices1();
	void addVertices2();
	void addIndices(int factor);
	void computeNormals(std::vector<MyVertexFormat> &vertices, std::vector<unsigned int> indices);

public:
	static const int BOTTOM = 12;				// Cel mai jos punct de pe rampa(coordonata y).

	Slope(SlopeType type, glm::vec3 pos);
	~Slope();									// Distruge geometria.

	void drawGeometry(unsigned int gl_program_shader);
	int getAngle();
	int getWidth();
	glm::vec3 getTop();
};

