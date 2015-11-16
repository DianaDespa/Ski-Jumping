// Despa Diana Alexandra 331CA

#include "Slope.h"


Slope::Slope(SlopeType type, glm::vec3 pos)
{
	this->type = type;
	color = glm::vec3(0.8, 0.8, 0.8);
	model_matrix = glm::mat4();
	if (type == ACTIVE) {
		model_matrix = glm::translate(model_matrix, pos);
		angle = 45;
		top = glm::vec3(43, 62, 0);
		width = 7;
	}
	else {
		model_matrix = glm::translate(model_matrix, pos);
		angle = 60;
		top = glm::vec3(56, 92, 0);
		width = 9;
	}
	createGeometry();
}

Slope::~Slope()
{
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);
	glDeleteBuffers(1, &ibo);
}

void Slope::createGeometry()
{
	if (type == ACTIVE)
		addVertices1();
	else
		addVertices2();

	Util::computeNormals(vertices, indices, model_matrix);

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

void Slope::addVertices1()
{
	// Rampa
	vertices.push_back(MyVertexFormat(glm::vec3(0, 50, 10), color));
	vertices.push_back(MyVertexFormat(glm::vec3(9, 50, 10), color));
	vertices.push_back(MyVertexFormat(glm::vec3(0, 52, 10), color));
	vertices.push_back(MyVertexFormat(glm::vec3(9, 52, 10), color));
	vertices.push_back(MyVertexFormat(glm::vec3(0, 60, 0), color));
	vertices.push_back(MyVertexFormat(glm::vec3(9, 60, 0), color));
	vertices.push_back(MyVertexFormat(glm::vec3(0, 62, 0), color));
	vertices.push_back(MyVertexFormat(glm::vec3(9, 62, 0), color));
	addIndices(0);
	vertices.push_back(MyVertexFormat(glm::vec3(0, 40, 20), color));
	vertices.push_back(MyVertexFormat(glm::vec3(9, 40, 20), color));
	vertices.push_back(MyVertexFormat(glm::vec3(0, 42, 20), color));
	vertices.push_back(MyVertexFormat(glm::vec3(9, 42, 20), color));
	vertices.push_back(MyVertexFormat(glm::vec3(0, 50, 10), color));
	vertices.push_back(MyVertexFormat(glm::vec3(9, 50, 10), color));
	vertices.push_back(MyVertexFormat(glm::vec3(0, 52, 10), color));
	vertices.push_back(MyVertexFormat(glm::vec3(9, 52, 10), color));
	addIndices(1);
	vertices.push_back(MyVertexFormat(glm::vec3(0, 30, 30), color));
	vertices.push_back(MyVertexFormat(glm::vec3(9, 30, 30), color));
	vertices.push_back(MyVertexFormat(glm::vec3(0, 32, 30), color));
	vertices.push_back(MyVertexFormat(glm::vec3(9, 32, 30), color));
	vertices.push_back(MyVertexFormat(glm::vec3(0, 40, 20), color));
	vertices.push_back(MyVertexFormat(glm::vec3(9, 40, 20), color));
	vertices.push_back(MyVertexFormat(glm::vec3(0, 42, 20), color));
	vertices.push_back(MyVertexFormat(glm::vec3(9, 42, 20), color));
	addIndices(2);
	vertices.push_back(MyVertexFormat(glm::vec3(0, 20, 40), color));
	vertices.push_back(MyVertexFormat(glm::vec3(9, 20, 40), color));
	vertices.push_back(MyVertexFormat(glm::vec3(0, 22, 40), color));
	vertices.push_back(MyVertexFormat(glm::vec3(9, 22, 40), color));
	vertices.push_back(MyVertexFormat(glm::vec3(0, 30, 30), color));
	vertices.push_back(MyVertexFormat(glm::vec3(9, 30, 30), color));
	vertices.push_back(MyVertexFormat(glm::vec3(0, 32, 30), color));
	vertices.push_back(MyVertexFormat(glm::vec3(9, 32, 30), color));
	addIndices(3);
	vertices.push_back(MyVertexFormat(glm::vec3(0, 10, 50), color));
	vertices.push_back(MyVertexFormat(glm::vec3(9, 10, 50), color));
	vertices.push_back(MyVertexFormat(glm::vec3(0, 12, 50), color));
	vertices.push_back(MyVertexFormat(glm::vec3(9, 12, 50), color));
	vertices.push_back(MyVertexFormat(glm::vec3(0, 20, 40), color));
	vertices.push_back(MyVertexFormat(glm::vec3(9, 20, 40), color));
	vertices.push_back(MyVertexFormat(glm::vec3(0, 22, 40), color));
	vertices.push_back(MyVertexFormat(glm::vec3(9, 22, 40), color));
	addIndices(4);

	// Coloana 1
	vertices.push_back(MyVertexFormat(glm::vec3(3, 32, 12), color));
	vertices.push_back(MyVertexFormat(glm::vec3(6, 32, 12), color));
	vertices.push_back(MyVertexFormat(glm::vec3(3, 48, 12), color));
	vertices.push_back(MyVertexFormat(glm::vec3(6, 48, 12), color));
	vertices.push_back(MyVertexFormat(glm::vec3(3, 32, 9), color));
	vertices.push_back(MyVertexFormat(glm::vec3(6, 32, 9), color));
	vertices.push_back(MyVertexFormat(glm::vec3(3, 51, 9), color));
	vertices.push_back(MyVertexFormat(glm::vec3(6, 51, 9), color));
	addIndices(5);
	vertices.push_back(MyVertexFormat(glm::vec3(3, 16, 12), color));
	vertices.push_back(MyVertexFormat(glm::vec3(6, 16, 12), color));
	vertices.push_back(MyVertexFormat(glm::vec3(3, 32, 12), color));
	vertices.push_back(MyVertexFormat(glm::vec3(6, 32, 12), color));
	vertices.push_back(MyVertexFormat(glm::vec3(3, 16, 9), color));
	vertices.push_back(MyVertexFormat(glm::vec3(6, 16, 9), color));
	vertices.push_back(MyVertexFormat(glm::vec3(3, 32, 9), color));
	vertices.push_back(MyVertexFormat(glm::vec3(6, 32, 9), color));
	addIndices(6);
	vertices.push_back(MyVertexFormat(glm::vec3(3, 0, 12), color));
	vertices.push_back(MyVertexFormat(glm::vec3(6, 0, 12), color));
	vertices.push_back(MyVertexFormat(glm::vec3(3, 16, 12), color));
	vertices.push_back(MyVertexFormat(glm::vec3(6, 16, 12), color));
	vertices.push_back(MyVertexFormat(glm::vec3(3, 0, 9), color));
	vertices.push_back(MyVertexFormat(glm::vec3(6, 0, 9), color));
	vertices.push_back(MyVertexFormat(glm::vec3(3, 16, 9), color));
	vertices.push_back(MyVertexFormat(glm::vec3(6, 16, 9), color));
	addIndices(7);

	// Coloana 2
	vertices.push_back(MyVertexFormat(glm::vec3(3, 13, 35), color));
	vertices.push_back(MyVertexFormat(glm::vec3(6, 13, 35), color));
	vertices.push_back(MyVertexFormat(glm::vec3(3, 26, 35), color));
	vertices.push_back(MyVertexFormat(glm::vec3(6, 26, 35), color));
	vertices.push_back(MyVertexFormat(glm::vec3(3, 13, 32), color));
	vertices.push_back(MyVertexFormat(glm::vec3(6, 13, 32), color));
	vertices.push_back(MyVertexFormat(glm::vec3(3, 29, 32), color));
	vertices.push_back(MyVertexFormat(glm::vec3(6, 29, 32), color));
	addIndices(8);
	vertices.push_back(MyVertexFormat(glm::vec3(3, 0, 35), color));
	vertices.push_back(MyVertexFormat(glm::vec3(6, 0, 35), color));
	vertices.push_back(MyVertexFormat(glm::vec3(3, 13, 35), color));
	vertices.push_back(MyVertexFormat(glm::vec3(6, 13, 35), color));
	vertices.push_back(MyVertexFormat(glm::vec3(3, 0, 32), color));
	vertices.push_back(MyVertexFormat(glm::vec3(6, 0, 32), color));
	vertices.push_back(MyVertexFormat(glm::vec3(3, 13, 32), color));
	vertices.push_back(MyVertexFormat(glm::vec3(6, 13, 32), color));
	addIndices(9);

	// Parapet
	color = glm::vec3(0.9, 0.9, 0.9);
	float val = 1/sqrt(2.0f);
	int count = 9;

	for (int i = 0; i < 3; ++i) {
		for (int j = 1; j <= 70; ++j) {
			vertices.push_back(MyVertexFormat(glm::vec3(0, 62 + (i - j) * val, (i + j) * val), color));
			vertices.push_back(MyVertexFormat(glm::vec3(1, 62 + (i - j) * val, (i + j) * val), color));
			vertices.push_back(MyVertexFormat(glm::vec3(0, 62 + (i - j + 1) * val, (i + j + 1) * val), color));
			vertices.push_back(MyVertexFormat(glm::vec3(1, 62 + (i - j + 1) * val, (i + j + 1) * val), color));
			vertices.push_back(MyVertexFormat(glm::vec3(0, 62 + (i - j + 1) * val, (i + j - 1) * val), color));
			vertices.push_back(MyVertexFormat(glm::vec3(1, 62 + (i - j + 1) * val, (i + j - 1) * val), color));
			vertices.push_back(MyVertexFormat(glm::vec3(0, 62 + (i - j + 2) * val, (i + j) * val), color));
			vertices.push_back(MyVertexFormat(glm::vec3(1, 62 + (i - j + 2) * val, (i + j) * val), color));
			addIndices(++count);
			vertices.push_back(MyVertexFormat(glm::vec3(8, 62 + (i - j) * val, (i + j) * val), color));
			vertices.push_back(MyVertexFormat(glm::vec3(9, 62 + (i - j) * val, (i + j) * val), color));
			vertices.push_back(MyVertexFormat(glm::vec3(8, 62 + (i - j + 1) * val, (i + j + 1) * val), color));
			vertices.push_back(MyVertexFormat(glm::vec3(9, 62 + (i - j + 1) * val, (i + j + 1) * val), color));
			vertices.push_back(MyVertexFormat(glm::vec3(8, 62 + (i - j + 1) * val, (i + j - 1) * val), color));
			vertices.push_back(MyVertexFormat(glm::vec3(9, 62 + (i - j + 1) * val, (i + j - 1) * val), color));
			vertices.push_back(MyVertexFormat(glm::vec3(8, 62 + (i - j + 2) * val, (i + j) * val), color));
			vertices.push_back(MyVertexFormat(glm::vec3(9, 62 + (i - j + 2) * val, (i + j) * val), color));
			addIndices(++count);
		}
	}
}

void Slope::addVertices2()
{
	// Rampa
	vertices.push_back(MyVertexFormat(glm::vec3(0, 70, 10), color));
	vertices.push_back(MyVertexFormat(glm::vec3(9, 70, 10), color));
	vertices.push_back(MyVertexFormat(glm::vec3(0, 72, 10), color));
	vertices.push_back(MyVertexFormat(glm::vec3(9, 72, 10), color));
	vertices.push_back(MyVertexFormat(glm::vec3(0, 90, 0), color));
	vertices.push_back(MyVertexFormat(glm::vec3(9, 90, 0), color));
	vertices.push_back(MyVertexFormat(glm::vec3(0, 92, 0), color));
	vertices.push_back(MyVertexFormat(glm::vec3(9, 92, 0), color));
	addIndices(0);
	vertices.push_back(MyVertexFormat(glm::vec3(0, 50, 20), color));
	vertices.push_back(MyVertexFormat(glm::vec3(9, 50, 20), color));
	vertices.push_back(MyVertexFormat(glm::vec3(0, 52, 20), color));
	vertices.push_back(MyVertexFormat(glm::vec3(9, 52, 20), color));
	vertices.push_back(MyVertexFormat(glm::vec3(0, 70, 10), color));
	vertices.push_back(MyVertexFormat(glm::vec3(9, 70, 10), color));
	vertices.push_back(MyVertexFormat(glm::vec3(0, 72, 10), color));
	vertices.push_back(MyVertexFormat(glm::vec3(9, 72, 10), color));
	addIndices(1);
	vertices.push_back(MyVertexFormat(glm::vec3(0, 30, 30), color));
	vertices.push_back(MyVertexFormat(glm::vec3(9, 30, 30), color));
	vertices.push_back(MyVertexFormat(glm::vec3(0, 32, 30), color));
	vertices.push_back(MyVertexFormat(glm::vec3(9, 32, 30), color));
	vertices.push_back(MyVertexFormat(glm::vec3(0, 50, 20), color));
	vertices.push_back(MyVertexFormat(glm::vec3(9, 50, 20), color));
	vertices.push_back(MyVertexFormat(glm::vec3(0, 52, 20), color));
	vertices.push_back(MyVertexFormat(glm::vec3(9, 52, 20), color));
	addIndices(2);
	vertices.push_back(MyVertexFormat(glm::vec3(0, 10, 40), color));
	vertices.push_back(MyVertexFormat(glm::vec3(9, 10, 40), color));
	vertices.push_back(MyVertexFormat(glm::vec3(0, 12, 40), color));
	vertices.push_back(MyVertexFormat(glm::vec3(9, 12, 40), color));
	vertices.push_back(MyVertexFormat(glm::vec3(0, 30, 30), color));
	vertices.push_back(MyVertexFormat(glm::vec3(9, 30, 30), color));
	vertices.push_back(MyVertexFormat(glm::vec3(0, 32, 30), color));
	vertices.push_back(MyVertexFormat(glm::vec3(9, 32, 30), color));
	addIndices(3);

	// Coloana 1
	vertices.push_back(MyVertexFormat(glm::vec3(3, 48, 12), color));
	vertices.push_back(MyVertexFormat(glm::vec3(6, 48, 12), color));
	vertices.push_back(MyVertexFormat(glm::vec3(3, 66, 12), color));
	vertices.push_back(MyVertexFormat(glm::vec3(6, 66, 12), color));
	vertices.push_back(MyVertexFormat(glm::vec3(3, 48, 9), color));
	vertices.push_back(MyVertexFormat(glm::vec3(6, 48, 9), color));
	vertices.push_back(MyVertexFormat(glm::vec3(3, 72, 9), color));
	vertices.push_back(MyVertexFormat(glm::vec3(6, 72, 9), color));
	addIndices(4);
	vertices.push_back(MyVertexFormat(glm::vec3(3, 32, 12), color));
	vertices.push_back(MyVertexFormat(glm::vec3(6, 32, 12), color));
	vertices.push_back(MyVertexFormat(glm::vec3(3, 48, 12), color));
	vertices.push_back(MyVertexFormat(glm::vec3(6, 48, 12), color));
	vertices.push_back(MyVertexFormat(glm::vec3(3, 32, 9), color));
	vertices.push_back(MyVertexFormat(glm::vec3(6, 32, 9), color));
	vertices.push_back(MyVertexFormat(glm::vec3(3, 48, 9), color));
	vertices.push_back(MyVertexFormat(glm::vec3(6, 48, 9), color));
	addIndices(5);
	vertices.push_back(MyVertexFormat(glm::vec3(3, 16, 12), color));
	vertices.push_back(MyVertexFormat(glm::vec3(6, 16, 12), color));
	vertices.push_back(MyVertexFormat(glm::vec3(3, 32, 12), color));
	vertices.push_back(MyVertexFormat(glm::vec3(6, 32, 12), color));
	vertices.push_back(MyVertexFormat(glm::vec3(3, 16, 9), color));
	vertices.push_back(MyVertexFormat(glm::vec3(6, 16, 9), color));
	vertices.push_back(MyVertexFormat(glm::vec3(3, 32, 9), color));
	vertices.push_back(MyVertexFormat(glm::vec3(6, 32, 9), color));
	addIndices(6);
	vertices.push_back(MyVertexFormat(glm::vec3(3, 0, 12), color));
	vertices.push_back(MyVertexFormat(glm::vec3(6, 0, 12), color));
	vertices.push_back(MyVertexFormat(glm::vec3(3, 16, 12), color));
	vertices.push_back(MyVertexFormat(glm::vec3(6, 16, 12), color));
	vertices.push_back(MyVertexFormat(glm::vec3(3, 0, 9), color));
	vertices.push_back(MyVertexFormat(glm::vec3(6, 0, 9), color));
	vertices.push_back(MyVertexFormat(glm::vec3(3, 16, 9), color));
	vertices.push_back(MyVertexFormat(glm::vec3(6, 16, 9), color));
	addIndices(7);

	// Coloana 2
	vertices.push_back(MyVertexFormat(glm::vec3(3, 13, 35), color));
	vertices.push_back(MyVertexFormat(glm::vec3(6, 13, 35), color));
	vertices.push_back(MyVertexFormat(glm::vec3(3, 20, 35), color));
	vertices.push_back(MyVertexFormat(glm::vec3(6, 20, 35), color));
	vertices.push_back(MyVertexFormat(glm::vec3(3, 13, 32), color));
	vertices.push_back(MyVertexFormat(glm::vec3(6, 13, 32), color));
	vertices.push_back(MyVertexFormat(glm::vec3(3, 26, 32), color));
	vertices.push_back(MyVertexFormat(glm::vec3(6, 26, 32), color));
	addIndices(8);
	vertices.push_back(MyVertexFormat(glm::vec3(3, 0, 35), color));
	vertices.push_back(MyVertexFormat(glm::vec3(6, 0, 35), color));
	vertices.push_back(MyVertexFormat(glm::vec3(3, 13, 35), color));
	vertices.push_back(MyVertexFormat(glm::vec3(6, 13, 35), color));
	vertices.push_back(MyVertexFormat(glm::vec3(3, 0, 32), color));
	vertices.push_back(MyVertexFormat(glm::vec3(6, 0, 32), color));
	vertices.push_back(MyVertexFormat(glm::vec3(3, 13, 32), color));
	vertices.push_back(MyVertexFormat(glm::vec3(6, 13, 32), color));
	addIndices(9);
}

void Slope::addIndices(int factor)
{
	unsigned int offset = 8 * factor;
	indices.push_back(0 + offset);	indices.push_back(1 + offset);	indices.push_back(2 + offset);
	indices.push_back(1 + offset);	indices.push_back(3 + offset);	indices.push_back(2 + offset);
	indices.push_back(2 + offset);	indices.push_back(3 + offset);	indices.push_back(7 + offset);
	indices.push_back(2 + offset);	indices.push_back(7 + offset);	indices.push_back(6 + offset);
	indices.push_back(1 + offset);	indices.push_back(7 + offset);	indices.push_back(3 + offset);
	indices.push_back(1 + offset);	indices.push_back(5 + offset);	indices.push_back(7 + offset);
	indices.push_back(6 + offset);	indices.push_back(7 + offset);	indices.push_back(4 + offset);
	indices.push_back(7 + offset);	indices.push_back(5 + offset);	indices.push_back(4 + offset);
	indices.push_back(0 + offset);	indices.push_back(4 + offset);	indices.push_back(1 + offset);
	indices.push_back(1 + offset);	indices.push_back(4 + offset);	indices.push_back(5 + offset);
	indices.push_back(2 + offset);	indices.push_back(6 + offset);	indices.push_back(4 + offset);
	indices.push_back(0 + offset);	indices.push_back(2 + offset);	indices.push_back(4 + offset);
}

void Slope::drawGeometry(unsigned int gl_program_shader)
{
	glUniformMatrix4fv(glGetUniformLocation(gl_program_shader, "model_matrix"), 1, false, glm::value_ptr(model_matrix));
	glUniformMatrix4fv(glGetUniformLocation(gl_program_shader, "scale_matrix"), 1, false, glm::value_ptr(glm::mat4()));
	glUniform1f(glGetUniformLocation(gl_program_shader, "is_cube"), false);
	glBindVertexArray(vao);
	glDrawElements(GL_TRIANGLES, num_indices, GL_UNSIGNED_INT, 0);
}

int Slope::getAngle()
{
	return angle;
}

int Slope::getWidth()
{
	return width;
}

glm::vec3 Slope::getTop()
{
	return top;
}