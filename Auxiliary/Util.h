// Despa Diana Alexandra 331CA

#pragma once
#include "MyVertexFormat.h"
#include <vector>

struct Util
{
	// Calculeaza normalele pentru toate varfurile.
	static void computeNormals(std::vector<MyVertexFormat> &vertices, std::vector<unsigned int> indices, glm::mat4 model_matrix)
	{
		glm::vec3 u, v, normal;
		glm::mat3 matrix;
		for (int i = 0; i < 3; ++i)
			for (int j = 0; j < 3; ++j)
				matrix[i][j] = model_matrix[i][j];

		for (int i = 0; i < vertices.size(); ++i) {
			vertices[i].face_count = 0;
		}
		for (int i = 0; i < indices.size(); i += 3) {
			u = vertices[indices[i + 1]].position - vertices[indices[i]].position;
			v = vertices[indices[i + 2]].position - vertices[indices[i]].position;
			u = matrix * u;
			v = matrix * v;
			normal.x = u.y * v.z - u.z * v.y;
			normal.y = u.z * v.x - u.x * v.z;
			normal.z = u.x * v.y - u.y * v.x;
			vertices[indices[i]].addFaceNormal(normal);
			vertices[indices[i + 1]].addFaceNormal(normal);
			vertices[indices[i + 2]].addFaceNormal(normal);
		}
		for (int i = 0; i < vertices.size(); ++i) {
			vertices[i].setNormal();
		}
	}
};

