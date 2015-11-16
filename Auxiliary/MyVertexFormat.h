// Despa Diana Alexandra 331CA

#pragma once
#include "..\dependente\glm\glm.hpp"

// Formatul vertecsilor: pozitie, culoare, normala la vertex.
struct MyVertexFormat
{
	glm::vec3 position, color, normal;
	unsigned int face_count;
	MyVertexFormat(const glm::vec3 &pos, const glm::vec3 &c) {
		position = pos;
		color = c;
		normal = glm::vec3(0, 0, 0);
		face_count = 0;
	}
	MyVertexFormat(const glm::vec3 &pos, const glm::vec3 &c, const glm::vec3 &n) {
		position = pos;
		color = c;
		normal = n;
		face_count = 0;
	}

	// Adauga normala unei fete la un vertor.
	void addFaceNormal(glm::vec3 normal) {
		face_count++;
		this->normal += normal;
	}

	// Face media normalelor fetelor care au un vertex comun pentru a obtine
	// normala la vertex.
	void setNormal() {
		normal.x = normal.x / face_count;
		normal.y = normal.y / face_count;
		normal.z = normal.z / face_count;
		if (normal != glm::vec3(0, 0, 0))
			normal = glm::normalize(normal);
	}
};