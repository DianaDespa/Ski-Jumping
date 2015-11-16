// Despa Diana Alexandra 331CA

#pragma once
#include "..\dependente\glew\glew.h"
#include "..\dependente\glm\glm.hpp"
#include "..\dependente\glm\gtc\type_ptr.hpp"
#include "..\dependente\glm\gtc\matrix_transform.hpp"
#include "..\lab_camera.hpp"
#include "Quad.h"
#include "Slope.h"
#include "ObjectBuilder.h"
#include <queue>

#define _USE_MATH_DEFINES
#include <math.h>

#define HALF_SIZE 0.5f
#define COEF_A -0.05
#define COEF_B 5
#define G 9.8

// Clasa ce reprezinta un personaj.
class Character
{
private:
	unsigned int vao, vbo, ibo, num_indices;	// Geometrie personaj.
	unsigned int vao_skis, vbo_skis, ibo_skis, num_indices_skis;	// Geometrie skiuri.
	unsigned int vao_cam, vbo_cam, ibo_cam, num_indices_cam;		// Geometrie camera TPS.
	glm::mat4 model_matrix, skis_model_matrix, cam_model_matrix, scale_matrix; // Matrici de modelare si scalare.
	// Vertecsi si indecsi pentru personaj, skiuri si camera TPS.
	std::vector<MyVertexFormat> vertices;
	std::vector<unsigned int> indices;
	std::vector<MyVertexFormat> vertices_skis;
	std::vector<unsigned int> indices_skis;
	std::vector<MyVertexFormat> vertices_cam;
	std::vector<unsigned int> indices_cam;
	Quad *quad;												// Quadul pe care se afla personajul.
	float ski_length;										// Lungime skiuri.
	CharacterType type;										// Tip personaj.
	bool is_on_slope, is_in_air, is_on_quad, is_inactive;	// Indica daca personajul este pe trambulina, in aer, pe quad sau a devenit inactiv.
	unsigned long time;										// Timpul curs de la inceperea miscarii pe rampa pana la saritura.
	float rotate_angle, slope_angle, ski_angle, cam_angle;	// Unghiuri de rotatie.
	glm::vec3 position, offset, tps_cam_offset;				// Pozitie si offset personaj, offset camera.
	std::pair<int, int> limits;								// Limitele rampei.
	std::vector<std::pair<int, int>> path;					// Calea pana la destinatie(pentru plasarea personajelor secundare).
	lab::Camera *fps_cam, *tps_cam;							// Camere.

	void createGeometry();						// Creeaza geometrie personaj.
	void createGeometrySkis();					// Creeaza geometrie schiuri.
	void createGeometryCam();					// Creeaza geometrie camera.

	void drawGeometrySkis(unsigned int gl_program_shader);	// Deseneaza geometrie skiuri.
	void rotate(int limit, int step);						// Roteste personaj in limita "limit" de grade, cu pasul "step".
	void makePathToFreeSpot();	// Calculeaza calea catre un patrat liber pe quad(pentru plasarea personajelor secundare).
	void moveToFreeSpot();		// Muta la un patrat liber pe quad(pentru plasarea personajelor secundare).

public:
	Character(CharacterType type, Slope *slope, ObjectBuilder *obj_builder, Quad *quad);
	~Character();				// Distruge geometria.

	void drawGeometry(unsigned int gl_program_shader);		// Deseneaza geometrie personaj.
	void drawGeometryCam(unsigned int gl_program_shader);	// Deseneaza geometrie camera TPS.
	CharacterType getType();				// Intoarce tip personaj.
	void rotateRight();						// Roteste dreapta.
	void rotateLeft();						// Roteste stanga.
	int moveForward();						// Muta inainte.
	void moveBack();						// Muta inapoi.
	void moveOnQuad(int move_direction);	// Muta pe quad in functie de directia "move_direction": 1 pentru inainte, -1 pentru inapoi.
	bool atDestination();					// Indica daca personajul poate deveni inactiv.
	void setInactive();						// Face personajul inactiv.
	bool onSlope();							// Indica daca personajul este pe trambulina.
	glm::vec3 getPosition();				// Intoarce pozitie personaj.
	void updateNormals();					// Actualizeaza normalele la obiecte.
	lab::Camera *getFpsCam();				// Camera FPS.
	lab::Camera *getTpsCam();				// Camera TPS.
};

