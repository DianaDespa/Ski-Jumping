// Despa Diana Alexandra 331CA

#include "ObjectBuilder.h"
#include "..\lab_mesh_loader.hpp"

std::vector<MyVertexFormat> convertVertices(std::vector<lab::VertexFormat> temp_vertices, glm::vec3 color)
{
	std::vector<MyVertexFormat> vertices;
	for (int i = 0; i < temp_vertices.size(); ++i) {
		vertices.push_back(MyVertexFormat(glm::vec3(
			temp_vertices[i].position_x,
			temp_vertices[i].position_y,
			temp_vertices[i].position_z),
			color));
	}
	return vertices;
}

ObjectBuilder::ObjectBuilder()
{
	glm::vec3 color = glm::vec3(0.3, 0.3, 0.3);
	std::vector<MyVertexFormat> vertices;
	std::vector<unsigned int> indices;
	vertices.push_back(MyVertexFormat(glm::vec3(-0.5, 0, 0.5), color));
	vertices.push_back(MyVertexFormat(glm::vec3(0.5, 0, 0.5), color));
	vertices.push_back(MyVertexFormat(glm::vec3(-0.5, 1, 0.5), color));
	vertices.push_back(MyVertexFormat(glm::vec3(0.5, 1, 0.5), color));
	vertices.push_back(MyVertexFormat(glm::vec3(-0.5, 0, -0.5), color));
	vertices.push_back(MyVertexFormat(glm::vec3(0.5, 0, -0.5), color));
	vertices.push_back(MyVertexFormat(glm::vec3(-0.5, 1, -0.5), color));
	vertices.push_back(MyVertexFormat(glm::vec3(0.5, 1, -0.5), color));

	indices.push_back(0); indices.push_back(1); indices.push_back(2);
	indices.push_back(1); indices.push_back(3); indices.push_back(2);
	indices.push_back(2); indices.push_back(3); indices.push_back(7);
	indices.push_back(2); indices.push_back(7); indices.push_back(6);
	indices.push_back(1); indices.push_back(7); indices.push_back(3);
	indices.push_back(1); indices.push_back(5); indices.push_back(7);
	indices.push_back(6); indices.push_back(7); indices.push_back(4);
	indices.push_back(7); indices.push_back(5); indices.push_back(4);
	indices.push_back(0); indices.push_back(4); indices.push_back(1);
	indices.push_back(1); indices.push_back(4); indices.push_back(5);
	indices.push_back(2); indices.push_back(6); indices.push_back(4);
	indices.push_back(0); indices.push_back(2); indices.push_back(4);

	objects.push_back(new Object(vertices, indices));

	std::vector<lab::VertexFormat> temp_vertices;
	indices.clear();
	_loadObjFile("resurse\\elsa.obj", temp_vertices, indices);
	objects.push_back(new Object(convertVertices(temp_vertices, color), indices));

	temp_vertices.clear();
	indices.clear();
	_loadObjFile("resurse\\bear.obj", temp_vertices, indices);
	objects.push_back(new Object(convertVertices(temp_vertices, color), indices));

	temp_vertices.clear();
	indices.clear();
	_loadObjFile("resurse\\male_low_poly.obj", temp_vertices, indices);
	objects.push_back(new Object(convertVertices(temp_vertices, color), indices));
}

ObjectBuilder::~ObjectBuilder()
{
}

void ObjectBuilder::get(CharacterType type, std::vector<MyVertexFormat> &vertices, std::vector<unsigned int> &indices)
{
	vertices = objects[type]->vertices;
	indices = objects[type]->indices;
}