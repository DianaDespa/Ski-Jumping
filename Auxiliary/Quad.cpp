// Despa Diana Alexandra 331CA

#include "Quad.h"


Quad::Quad(unsigned int size)
{
	this->size = size;
	model_matrix = glm::mat4();
	std::vector<bool> line;
	for (int i = 0; i < size; ++i) {
		line.push_back(false);
	}
	for (int i = 0; i < size; ++i) {
		occupied.push_back(line);
	}

	// Marchez ca fiind ocupate locurile de pe quad unde se afla coloanele care
	// sustin rampele.
	for (int i = 42; i <= 44; ++i)
		for (int j = 9; j <= 11; ++j)
			occupied[i][j] = true;
	for (int i = 42; i <= 44; ++i)
		for (int j = 32; j <= 34; ++j)
			occupied[i][j] = true;
	for (int i = 55; i <= 56; ++i)
		for (int j = 9; j <= 11; ++j)
			occupied[i][j] = true;
	for (int i = 55; i <= 56; ++i)
		for (int j = 32; j <= 34; ++j)
			occupied[i][j] = true;
	createGeometry();
}


Quad::~Quad()
{
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);
	glDeleteBuffers(1, &ibo);
}

unsigned int Quad::getSize()
{
	return size;
}

void Quad::createGeometry()
{
	glm::vec3 color = glm::vec3(0, 0.9, 0.9);
	std::vector<MyVertexFormat> vertices;
	for (int i = 0; i <= size; ++i) {
		for (int j = 0; j <= size; ++j) {
			vertices.push_back(MyVertexFormat(glm::vec3(i, 0, j), color, glm::normalize(glm::vec3(0, 1, 0))));
		}
	}

	std::vector<unsigned int> indices;
	for (int i = 0; i < size; ++i) {
		for (int j = 0; j < size; ++j) {
			indices.push_back((i + 1) * (size + 1) + j); 
			indices.push_back(i * (size + 1) + j); 
			indices.push_back((i + 1) * (size + 1) + j + 1);
			indices.push_back((i + 1) * (size + 1) + j + 1);
			indices.push_back(i * (size + 1) + j);
			indices.push_back(i * (size + 1) + j + 1);
		}
	}
	num_indices = indices.size();

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(MyVertexFormat)*(vertices.size()), &vertices[0], GL_STATIC_DRAW);
	glGenBuffers(1, &ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * num_indices, &indices[0], GL_STATIC_DRAW);
	
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(MyVertexFormat), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(MyVertexFormat), (void*)(sizeof(float) * 3));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(MyVertexFormat), (void*)(2 * sizeof(float) * 3));
}

void Quad::drawGeometry(unsigned int gl_program_shader)
{
	glUniformMatrix4fv(glGetUniformLocation(gl_program_shader, "model_matrix"), 1, false, glm::value_ptr(model_matrix));
	glUniform1f(glGetUniformLocation(gl_program_shader, "is_cube"), false);
	glBindVertexArray(vao);
	glDrawElements(GL_TRIANGLES, num_indices, GL_UNSIGNED_INT, 0);
}

void Quad::setOccupied(int i, int j)
{
	occupied[i][j] = true;
}

bool Quad::isOccupied(int i, int j)
{
	return occupied[i][j];
}

std::pair<int, int> Quad::getFirstFree()
{
	for (int i = size - 2; i >= 0; --i)
		for (int j = size - 2; j >= 0; --j)
			if (!occupied[j][i])
				return std::make_pair(j, i);
	return std::make_pair(-1, -1);
}