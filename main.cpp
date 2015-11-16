// Despa Diana Alexandra 331CA

#include "lab_shader_loader.hpp"
#include "lab_glut.hpp"
#include "lab_camera.hpp"
#include <ctime>
#include "Auxiliary/Quad.h"
#include "Auxiliary/Slope.h"
#include "Auxiliary/Character.h"
#include "Auxiliary/ObjectBuilder.h"

class Tema : public lab::glut::WindowListener{

private:
	glm::mat4 projection_matrix;		// Matrice 4x4 pt proiectie.
	std::vector<lab::Camera*> cams;		// Camere.
	int cam_index;						// Indexul camerei curente.
	Quad *quad;
	ObjectBuilder *obj_build;
	std::vector<Character*> characters;				// Personaje.
	Character *main_character, *second_character;
	Slope *main_slope, *second_slope;				// Trambuline.
	unsigned int gl_program_shader;					// Id-ul de opengl al obiectului de tip program shader.
	unsigned int screen_width, screen_height;
	unsigned int quad_size;

	// Caractertici pentru iluminare.
	glm::vec3 light_position;
	unsigned int material_shininess;
	float material_kd;
	float material_ks;

	// Indica daca personajele sunt speciale si daca desenarea s-a oprit.
	bool special_mode, stop;

	// Caracteristici pentru efectul de cutremur.
	float x_quake, amplitude, frequency;

public:

	Tema() {
		// Setari pentru desenare, clear color seteaza culoarea de clear pentru ecran (format R,G,B,A)
		glClearColor(0.03, 0.008, 0.2, 1);
		glClearDepth(1);			// Clear depth si depth test (nu le studiem momentan, dar avem nevoie de ele!)
		glEnable(GL_DEPTH_TEST);	// Sunt folosite pentru a determina obiectele cele mai apropiate de camera (la curs: algoritmul pictorului, algoritmul zbuffer)
		
		// Incarca un shader din fisiere si gaseste locatiile matricilor relativ la programul creat
		gl_program_shader = lab::loadShader("shadere\\shader_vertex.glsl", "shadere\\shader_fragment.glsl");

		// Desenare wireframe
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glLineWidth(1);
		glPointSize(1);

		// Initializare object builder, quad si trambuline.
		obj_build = new ObjectBuilder();
		quad_size = 100;
		quad = new Quad(quad_size);
		main_slope = new Slope(ACTIVE, glm::vec3(quad_size / 2 - 11, 0, 0));
		second_slope = new Slope(PASSIVE, glm::vec3(quad_size / 2 + 2, 0, 0));

		// Lumina & material.
		light_position = glm::vec3(0, 100, 50);
		material_shininess = 100;
		material_kd = 0.5;
		material_ks = 0.5;

		special_mode = false;
		init();
	}

	~Tema() {
		glDeleteProgram(gl_program_shader);
		delete quad;
		for (int i = 0; i < characters.size(); ++i) {
			delete characters[i];
		}
		delete main_slope;
		delete second_slope;
		delete obj_build;
		for (int i = 0; i < cams.size(); ++i) {
			delete cams[i];
		}
	}

	// Initializeaza primele personaje, variabilele pentru cutremur si camerele.
	void init() {
		if (!special_mode) {
			main_character = new Character(MAIN_ACTIVE, main_slope, obj_build, quad);
			second_character = new Character(SECOND_ACTIVE, second_slope, obj_build, quad);
		}
		else {
			main_character = new Character(MAIN_SPECIAL, main_slope, obj_build, quad);
			second_character = new Character(SECOND_SPECIAL, second_slope, obj_build, quad);
		}
		characters.push_back(main_character);
		characters.push_back(second_character);

		x_quake = -1;
		amplitude = 0;
		frequency = 0;
		srand(time(NULL));
		stop = false;

		cam_index = 0;
		cams.push_back(new lab::Camera());
		cams.push_back(new lab::Camera());
		cams.push_back(new lab::Camera());
		cams.push_back(new lab::Camera());
		cams.push_back(new lab::Camera());
		cams.push_back(new lab::Camera());
		cams.push_back(new lab::Camera());
		cams[1]->set(glm::vec3(10, 1, quad->getSize() - 10), glm::vec3(43, 30, 30), glm::vec3(0, 1, 0));
		cams[3]->set(glm::vec3(quad->getSize() - 10, 1, quad->getSize() - 10), glm::vec3(56, 40, 30), glm::vec3(0, 1, 0));
		cams[6]->set(glm::vec3(50, 100, 50), glm::vec3(49, 0, 49), glm::vec3(0, 100, 50));
		cams[6]->rotateFPSoX(-1.0f);

		delete cams[0];
		delete cams[2];
		delete cams[4];
		delete cams[5];
		cams[0] = main_character->getFpsCam();
		cams[2] = second_character->getFpsCam();
		cams[4] = main_character->getTpsCam();
		cams[5] = second_character->getTpsCam();
	}
	
	// Functie chemata inainte de a incepe cadrul de desenare, o folosim ca sa updatam situatia scenei (modelam/simulam scena)
	void notifyBeginFrame() {
		static float step;
		if (!stop) {
			float angle = 0.0005f;
			glm::vec3 lp = light_position;
			light_position.x = lp.x * cos(angle) - lp.y * sin(angle);
			light_position.y = lp.x * sin(angle) + lp.y * cos(angle);
			for (int i = 0; i < characters.size(); ++i) {
				if (characters[i] == main_character) {
					if (!main_character->onSlope()) {
						// Cutremurul se opreste daca personajul nu mai este pe trambulina.
						x_quake = -1;
					} else if (x_quake >= 0) {
						// In timpul unui cutremur modific coordonata x.
						if (x_quake >= 0.995)
							step = -0.005;
						if (x_quake <= 0.005)
							step = 0.005;
						x_quake += step;
					}
				} 
				else if (characters[i]->getType() == SECOND_ACTIVE || characters[i]->getType() == SECOND_SPECIAL) {
					// Desenarea se opreste daca personajul nu se poate muta inainte.
					stop = characters[i]->moveForward() == 1;
					if (second_character->atDestination()) {
						// Daca personajul secundar a ajuns la destinatie, il fac pasiv si il ilocuiesc cu unul nou.
						second_character = new Character(characters[i]->getType(), second_slope, obj_build, quad);
						characters.push_back(second_character);
						delete cams[2];
						cams[2] = second_character->getFpsCam();
						delete cams[5];
						cams[5] = second_character->getTpsCam();
					}
				}
			}
		}
	}

	// Functia de afisare (lucram cu banda grafica)
	void notifyDisplayFrame() {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glViewport(0, 0, screen_width, screen_height);
		glUseProgram(gl_program_shader);
				
		glUniformMatrix4fv(glGetUniformLocation(gl_program_shader, "view_matrix"), 1, false, glm::value_ptr(cams[cam_index]->getViewMatrix()));
		glUniformMatrix4fv(glGetUniformLocation(gl_program_shader, "projection_matrix"), 1, false, glm::value_ptr(projection_matrix));
		glUniform3f(glGetUniformLocation(gl_program_shader, "light_position"), light_position.x, light_position.y, light_position.z);
		glUniform3f(glGetUniformLocation(gl_program_shader, "eye_position"),
			cams[cam_index]->getPosition().x, cams[cam_index]->getPosition().y, cams[cam_index]->getPosition().z);
		glUniform1i(glGetUniformLocation(gl_program_shader, "material_shininess"), material_shininess);
		glUniform1f(glGetUniformLocation(gl_program_shader, "material_kd"), material_kd);
		glUniform1f(glGetUniformLocation(gl_program_shader, "material_ks"), material_ks);
		glUniform1f(glGetUniformLocation(gl_program_shader, "x_quake"), -1);

		quad->drawGeometry(gl_program_shader);
		
		if(x_quake > -1) {
			// In timpul unui cutremur trimit variabilele uniforme corespunzatoare
			// la shader.
			glUniform1f(glGetUniformLocation(gl_program_shader, "x_quake"), x_quake);
			float y_quake = amplitude * cos(frequency * x_quake),
				z_quake = amplitude * sin(frequency * x_quake);
			glUniform1f(glGetUniformLocation(gl_program_shader, "y_quake"), y_quake);
			glUniform1f(glGetUniformLocation(gl_program_shader, "z_quake"), z_quake);
		}
		
		main_character->drawGeometry(gl_program_shader);
		main_slope->drawGeometry(gl_program_shader);

		// Restul obiectelor nu tin cont de cutremur. 
		glUniform1f(glGetUniformLocation(gl_program_shader, "x_quake"), -1);
		for (int i = 0; i < characters.size(); ++i) {
			if (characters[i] != main_character)
				characters[i]->drawGeometry(gl_program_shader);
			if (characters[i] == main_character || characters[i] == second_character)
				characters[i]->drawGeometryCam(gl_program_shader);
		}
		second_slope->drawGeometry(gl_program_shader);
	}

	// Functie chemata dupa ce am terminat cadrul de desenare (poate fi folosita pt modelare/simulare)
	void notifyEndFrame(){}
	
	// Functei care e chemata cand se schimba dimensiunea ferestrei initiale
	void notifyReshape(int width, int height, int previous_width, int previous_height) {
		if (height == 0)
			height = 1;
		screen_width = width;
		screen_height = height;
		float aspect = width / height;
		projection_matrix = glm::perspective(75.0f, aspect, 0.1f, 10000.0f);
	}

	// Tasta apasata
	void notifyKeyPressed(unsigned char key_pressed, int mouse_x, int mouse_y) {
		
		if(key_pressed == 27) lab::glut::close();	// ESC inchide glut.
		if(key_pressed == 32) {
			// SPACE reincarca shaderul si recalculeaza locatiile (offseti/pointeri)
			glDeleteProgram(gl_program_shader);
			gl_program_shader = lab::loadShader("shadere\\shader_vertex.glsl", "shadere\\shader_fragment.glsl");
		}
		if (key_pressed == 'a') {
			if (main_character->atDestination()) {
				// Plasez personaj principal si generez unul nou.
				glm::vec3 pos = main_character->getPosition();
				quad->setOccupied((int)ceil(pos.x - 1), (int)ceil(pos.z - 1));
				main_character->setInactive();
				main_character = new Character(MAIN_ACTIVE, main_slope, obj_build, quad);
				characters.push_back(main_character);
				delete cams[0];
				cams[0] = main_character->getFpsCam();
				delete cams[4];
				cams[4] = main_character->getTpsCam();
			}
		}
		if (key_pressed == 'r') {	// Opreste cutremur.
			x_quake = -1;
		}
		if (key_pressed == 'e') {	// Incepe cutremur.
			if (x_quake <= -1) {
				amplitude = 0.2 + ((double)rand() / RAND_MAX) * 0.8;	// intre 0.2 si 1.
				frequency = rand() % 50;
				x_quake = 0;
			}

		}
		if (key_pressed == 'c') {	// Comuta intre camere.
			cam_index = (cam_index + 1) % 7;
		}
		if (key_pressed == 'w') {	// Desenare wireframe.
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		}
		if (key_pressed == 's') {	// Desenare solid.
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}
		// Taste pentru deplasarea camerei FPS de sus.
		if (key_pressed == 't') {
			if (cam_index == 6)
				cams[6]->translateForward(1.0f);
		}
		if (key_pressed == 'y') {
			if (cam_index == 6)
				cams[6]->translateForward(-1.0f);
		}
		if (key_pressed == 'u') {
			if (cam_index == 6)
				cams[6]->translateUpword(1.0f);
		}
		if (key_pressed == 'i') {
			if (cam_index == 6)
				cams[6]->translateUpword(-1.0f);
		}
		if (key_pressed == 'o') {
			if (cam_index == 6)
				cams[6]->translateRight(-1.0f);
		}
		if (key_pressed == 'p') {
			if (cam_index == 6)
				cams[6]->translateRight(1.0f);
		}
		if (key_pressed == '1') {
			if (cam_index == 6)
				cams[6]->rotateFPSoX(1.0f);
		}
		if (key_pressed == '2') {
			if (cam_index == 6)
				cams[6]->rotateFPSoX(-1.0f);
		}
		if (key_pressed == '3') {
			if (cam_index == 6)
				cams[6]->rotateFPSoY(1.0f);
		}
		if (key_pressed == '4') {
			if (cam_index == 6)
				cams[6]->rotateFPSoY(-1.0f);
		}
		if (key_pressed == '5') {
			if (cam_index == 6)
				cams[6]->rotateFPSoZ(1.0f);
		}
		if (key_pressed == '6') {
			if (cam_index == 6)
				cams[6]->rotateFPSoZ(-1.0f);
		}
		if (key_pressed == 'q') {			// Schimba modul special cu modul normal si invers.
			special_mode = !special_mode;
			for (int i = 0; i < characters.size(); ++i) {
				delete characters[i];
			}
			for (int i = 0; i < cams.size(); ++i) {
				delete cams[i];
			}
			cams.clear();
			characters.clear();
			init();
		}
	}

	// Tasta ridicata
	void notifyKeyReleased(unsigned char key_released, int mouse_x, int mouse_y) {
	}

	// Tasta speciala (up/down/F1/F2..) apasata
	void notifySpecialKeyPressed(int key_pressed, int mouse_x, int mouse_y) {
		if (key_pressed == GLUT_KEY_F1)
			lab::glut::enterFullscreen();
		if (key_pressed == GLUT_KEY_F2)
			lab::glut::exitFullscreen();
		if (key_pressed == GLUT_KEY_RIGHT)		// Roteste dreapta.
			if (!stop)
				main_character->rotateRight();
		if (key_pressed == GLUT_KEY_LEFT)		// Roteste stanga.
			if (!stop)
				main_character->rotateLeft();
		if (key_pressed == GLUT_KEY_UP)			// Muta inainte.
			stop = stop || main_character->moveForward() == 1;
		if (key_pressed == GLUT_KEY_DOWN)		// Muta inapoi.
			main_character->moveBack();
	}

	// Tasta speciala ridicata
	void notifySpecialKeyReleased(int key_released, int mouse_x, int mouse_y){
		if (key_released == GLUT_KEY_RIGHT ||
			key_released == GLUT_KEY_LEFT ||
			key_released == GLUT_KEY_UP ||
			key_released == GLUT_KEY_DOWN)
			main_character->updateNormals();
	}

	// Drag cu mouse-ul
	void notifyMouseDrag(int mouse_x, int mouse_y){}
	
	// Am miscat mouseul (fara sa apas vreun buton)
	void notifyMouseMove(int mouse_x, int mouse_y){}
	
	// Am apasat pe un boton
	void notifyMouseClick(int button, int state, int mouse_x, int mouse_y){}
	
	// Scroll cu mouse-ul
	void notifyMouseScroll(int wheel, int direction, int mouse_x, int mouse_y) {
		std::cout<<"Mouse scroll"<<std::endl;
	}
};

int main() {
	// Initializeaza GLUT (fereastra + input + context OpenGL)
	lab::glut::WindowInfo window(std::string("Tema 3 - Ski Jumping"),800,600,100,100,true);
	lab::glut::ContextInfo context(3,3,false);
	lab::glut::FramebufferInfo framebuffer(true,true,true,true);
	lab::glut::init(window,context, framebuffer);

	// Initializeaza GLEW (ne incarca functiile openGL, altfel ar trebui sa facem asta manual!)
	glewExperimental = true;
	glewInit();
	std::cout<<"GLEW:initializare"<<std::endl;

	// Creem clasa noastra si o punem sa asculte evenimentele de la GLUT
	// DUPA GLEW!!! ca sa avem functiile de OpenGL incarcate inainte sa ii fie apelat constructorul (care creeaza obiecte OpenGL)
	Tema tema;
	lab::glut::setListener(&tema);

	// Taste
	std::cout<<"Taste:"<<std::endl<<"\tESC ... iesire"<<std::endl<<"\tSPACE ... reincarca shadere"<<std::endl<<"\ti ... toggle wireframe"<<std::endl<<"\to ... reseteaza camera"<<std::endl;
	std::cout<<"\tw ... translatie camera in fata"<<std::endl<<"\ts ... translatie camera inapoi"<<std::endl;
	std::cout<<"\ta ... translatie camera in stanga"<<std::endl<<"\td ... translatie camera in dreapta"<<std::endl;
	std::cout<<"\tr ... translatie camera in sus"<<std::endl<<"\tf ... translatie camera jos"<<std::endl;
	std::cout<<"\tq ... rotatie camera FPS pe Oy, minus"<<std::endl<<"\te ... rotatie camera FPS pe Oy, plus"<<std::endl;
	std::cout<<"\tz ... rotatie camera FPS pe Oz, minus"<<std::endl<<"\tc ... rotatie camera FPS pe Oz, plus"<<std::endl;

	// Run
	lab::glut::run();

	return 0;
}