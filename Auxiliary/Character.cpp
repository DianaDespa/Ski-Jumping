// Despa Diana Alexandra 331CA

#include "Character.h"

Character::Character(CharacterType type, Slope *slope, ObjectBuilder *obj_builder, Quad *quad)
{
	this->quad = quad;
	this->type = type;
	time = 0;
	is_on_slope = true;
	is_in_air = false;
	is_on_quad = false;
	is_inactive = false;
	rotate_angle = 0;
	ski_length = 2.5f;
	position = slope->getTop();
	slope_angle = slope->getAngle();
	ski_angle = slope->getAngle();
	offset = glm::vec3(0, 0, 0);
	limits.first = position.x - slope->getWidth() / 2;
	limits.second = position.x + slope->getWidth() / 2;
	scale_matrix = glm::mat4();
	
	model_matrix = glm::mat4();
	skis_model_matrix = glm::mat4();
	position += glm::vec3(HALF_SIZE, -sin(M_PI * slope_angle / 180), HALF_SIZE);
	if (type == MAIN_ACTIVE || type == MAIN_SPECIAL)
		position += glm::vec3(0, 0.3, 0);

	model_matrix = glm::translate(model_matrix, position);
	model_matrix = glm::rotate(model_matrix, slope_angle, glm::vec3(1, 0, 0));
	skis_model_matrix = glm::translate(skis_model_matrix, position);
	skis_model_matrix = glm::rotate(skis_model_matrix, slope_angle, glm::vec3(1, 0, 0));

	obj_builder->get(type, vertices, indices);
	createGeometry();
	createGeometrySkis();
	createGeometryCam();

	fps_cam = new lab::Camera();
	fps_cam->set(position, position + glm::vec3(0, 0, 1), glm::vec3(0, 1, 0));
	fps_cam->rotateFPSoZ(-slope_angle);
	fps_cam->translateUpword(HALF_SIZE);
	fps_cam->translateForward(HALF_SIZE);

	tps_cam = new lab::Camera();
	tps_cam_offset = glm::vec3(-5, 7, 0);
	tps_cam->set(position + tps_cam_offset, position + glm::vec3(0, HALF_SIZE, 0), glm::vec3(0, 1, 0));
	cam_angle = atan((float)7 / 5) * 180 / M_PI;
	cam_model_matrix = glm::mat4();
	cam_model_matrix = glm::translate(cam_model_matrix, tps_cam->getPosition());
	cam_model_matrix = glm::rotate(cam_model_matrix, -cam_angle, glm::vec3(0, 0, 1));
}

Character::~Character()
{
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);
	glDeleteBuffers(1, &ibo);
	glDeleteVertexArrays(1, &vao_skis);
	glDeleteBuffers(1, &vbo_skis);
	glDeleteBuffers(1, &ibo_skis);
	glDeleteVertexArrays(1, &vao_cam);
	glDeleteBuffers(1, &vbo_cam);
	glDeleteBuffers(1, &ibo_cam);
}

CharacterType Character::getType()
{
	return type;
}

void Character::createGeometry()
{
	if (type == SECOND_SPECIAL)
		scale_matrix = glm::scale(scale_matrix, glm::vec3(0.125, 0.125, 0.125));

	Util::computeNormals(vertices, indices, model_matrix);

	num_indices = indices.size();
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glGenBuffers(1, &ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * num_indices, &indices[0], GL_STATIC_DRAW);
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(MyVertexFormat)*(vertices.size()), &vertices[0], GL_STREAM_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(MyVertexFormat), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(MyVertexFormat), (void*)(sizeof(float) * 3));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(MyVertexFormat), (void*)(2 * sizeof(float) * 3));
}

void Character::createGeometrySkis()
{
	vertices_skis.push_back(MyVertexFormat(glm::vec3(-0.3, 0.01, -0.5), glm::vec3(0, 1, 1)));
	vertices_skis.push_back(MyVertexFormat(glm::vec3(-0.15, 0.01, -0.5), glm::vec3(1, 0, 1)));
	vertices_skis.push_back(MyVertexFormat(glm::vec3(-0.3, 0.01, ski_length - 0.5), glm::vec3(1, 1, 0)));
	vertices_skis.push_back(MyVertexFormat(glm::vec3(-0.15, 0.01, ski_length - 0.5), glm::vec3(0, 0, 0)));
	indices_skis.push_back(0); indices_skis.push_back(2); indices_skis.push_back(3);
	indices_skis.push_back(0); indices_skis.push_back(1); indices_skis.push_back(3);
	vertices_skis.push_back(MyVertexFormat(glm::vec3(0.15, 0.01, -0.5), glm::vec3(0, 1, 1)));
	vertices_skis.push_back(MyVertexFormat(glm::vec3(0.3, 0.01, -0.5), glm::vec3(1, 0, 1)));
	vertices_skis.push_back(MyVertexFormat(glm::vec3(0.15, 0.01, ski_length - 0.5), glm::vec3(1, 1, 0)));
	vertices_skis.push_back(MyVertexFormat(glm::vec3(0.3, 0.01, ski_length - 0.5), glm::vec3(0, 0, 0)));
	indices_skis.push_back(4); indices_skis.push_back(6); indices_skis.push_back(7);
	indices_skis.push_back(4); indices_skis.push_back(5); indices_skis.push_back(7);

	Util::computeNormals(vertices_skis, indices_skis, skis_model_matrix);

	num_indices_skis = indices_skis.size();
	glGenVertexArrays(1, &vao_skis);
	glBindVertexArray(vao_skis);
	glGenBuffers(1, &ibo_skis);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_skis);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * num_indices_skis, &indices_skis[0], GL_STATIC_DRAW);
	glGenBuffers(1, &vbo_skis);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_skis);
	glBufferData(GL_ARRAY_BUFFER, sizeof(MyVertexFormat)*(vertices_skis.size()), &vertices_skis[0], GL_STREAM_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(MyVertexFormat), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(MyVertexFormat), (void*)(sizeof(float) * 3));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(MyVertexFormat), (void*)(2 * sizeof(float) * 3));
}

void Character::createGeometryCam()
{
	glm::vec3 color(0, 1, 0);
	vertices_cam.push_back(MyVertexFormat(glm::vec3(-1, 0, 0.125), color));
	vertices_cam.push_back(MyVertexFormat(glm::vec3(0, 0, 0.125), color));
	vertices_cam.push_back(MyVertexFormat(glm::vec3(-1, 0.25, 0.125), color));
	vertices_cam.push_back(MyVertexFormat(glm::vec3(0, 0.25, 0.125), color));
	vertices_cam.push_back(MyVertexFormat(glm::vec3(-1, 0, -0.125), color));
	vertices_cam.push_back(MyVertexFormat(glm::vec3(0, 0, -0.125), color));
	vertices_cam.push_back(MyVertexFormat(glm::vec3(-1, 0.25, -0.125), color));
	vertices_cam.push_back(MyVertexFormat(glm::vec3(0, 0.25, -0.125), color));

	indices_cam.push_back(0); indices_cam.push_back(1); indices_cam.push_back(2);
	indices_cam.push_back(1); indices_cam.push_back(3); indices_cam.push_back(2);
	indices_cam.push_back(2); indices_cam.push_back(3); indices_cam.push_back(7);
	indices_cam.push_back(2); indices_cam.push_back(7); indices_cam.push_back(6);
	indices_cam.push_back(1); indices_cam.push_back(7); indices_cam.push_back(3);
	indices_cam.push_back(1); indices_cam.push_back(5); indices_cam.push_back(7);
	indices_cam.push_back(6); indices_cam.push_back(7); indices_cam.push_back(4);
	indices_cam.push_back(7); indices_cam.push_back(5); indices_cam.push_back(4);
	indices_cam.push_back(0); indices_cam.push_back(4); indices_cam.push_back(1);
	indices_cam.push_back(1); indices_cam.push_back(4); indices_cam.push_back(5);
	indices_cam.push_back(2); indices_cam.push_back(6); indices_cam.push_back(4);
	indices_cam.push_back(0); indices_cam.push_back(2); indices_cam.push_back(4);

	num_indices_cam = indices_cam.size();
	glGenVertexArrays(1, &vao_cam);
	glBindVertexArray(vao_cam);
	glGenBuffers(1, &ibo_cam);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_cam);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * num_indices_cam, &indices_cam[0], GL_STATIC_DRAW);
	glGenBuffers(1, &vbo_cam);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_cam);
	glBufferData(GL_ARRAY_BUFFER, sizeof(MyVertexFormat)*(vertices_cam.size()), &vertices_cam[0], GL_STREAM_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(MyVertexFormat), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(MyVertexFormat), (void*)(sizeof(float) * 3));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(MyVertexFormat), (void*)(2 * sizeof(float) * 3));
}

void Character::drawGeometry(unsigned int gl_program_shader)
{
	if (!is_inactive && offset != glm::vec3(0, 0, 0) && (type == MAIN_ACTIVE || type == MAIN_SPECIAL))
		time++;
	glUniform1f(glGetUniformLocation(gl_program_shader, "time"), time);
	glUniformMatrix4fv(glGetUniformLocation(gl_program_shader, "model_matrix"), 1, false, glm::value_ptr(model_matrix));
	glUniformMatrix4fv(glGetUniformLocation(gl_program_shader, "scale_matrix"), 1, false, glm::value_ptr(scale_matrix));
	glUniform1f(glGetUniformLocation(gl_program_shader, "is_main"), type == MAIN_ACTIVE || type == MAIN_SPECIAL);

	glBindVertexArray(vao);
	glDrawElements(GL_TRIANGLES, num_indices, GL_UNSIGNED_INT, 0);
	drawGeometrySkis(gl_program_shader);
}

void Character::drawGeometrySkis(unsigned int gl_program_shader)
{
	glUniformMatrix4fv(glGetUniformLocation(gl_program_shader, "scale_matrix"), 1, false, glm::value_ptr(glm::mat4()));
	glUniformMatrix4fv(glGetUniformLocation(gl_program_shader, "model_matrix"), 1, false, glm::value_ptr(skis_model_matrix));
	glUniform1f(glGetUniformLocation(gl_program_shader, "is_main"), false);
	glBindVertexArray(vao_skis);
	glDrawElements(GL_TRIANGLES, num_indices_skis, GL_UNSIGNED_INT, 0);
}

void Character::drawGeometryCam(unsigned int gl_program_shader)
{
	glUniformMatrix4fv(glGetUniformLocation(gl_program_shader, "scale_matrix"), 1, false, glm::value_ptr(glm::mat4()));
	glUniformMatrix4fv(glGetUniformLocation(gl_program_shader, "model_matrix"), 1, false, glm::value_ptr(cam_model_matrix));
	glUniform1f(glGetUniformLocation(gl_program_shader, "is_main"), false);
	glBindVertexArray(vao_cam);
	glDrawElements(GL_TRIANGLES, num_indices_cam, GL_UNSIGNED_INT, 0);
}

void Character::rotateRight()
{
	if (is_on_slope)
		rotate(60, -2);
	else
		rotate(360, -2);
}

void Character::rotateLeft()
{
	if (is_on_slope)
		rotate(60, 2);
	else
		rotate(360, 2);
}

void Character::rotate(int limit, int step)
{
	if (abs(rotate_angle + step) <= limit && (is_on_slope || is_on_quad)) {
		rotate_angle = ((int)rotate_angle + step) % 360;
		model_matrix = glm::rotate(model_matrix, (float)step, glm::vec3(0, 1, 0));
		skis_model_matrix = glm::rotate(skis_model_matrix, (float)step, glm::vec3(0, 1, 0));
		fps_cam->rotateTPSoY(step, -HALF_SIZE);
	}
}

int Character::moveForward()
{
	float speed = (type == MAIN_ACTIVE || type == MAIN_SPECIAL) ? 1 : 2,
		slowdown = (type == MAIN_ACTIVE || type == MAIN_SPECIAL) ? 1 : 0.025;

	if (is_on_slope) {
		float rad_angle = M_PI * rotate_angle / 180;
		offset = glm::vec3(sin(rad_angle) * slowdown, -speed * slowdown, slowdown);
		if (offset.x + position.x + ski_length * sin(rad_angle) - HALF_SIZE <= limits.first) {
			rotateLeft();
			moveForward();
			return 0;
		}
		if (offset.x + position.x + ski_length * sin(rad_angle) - HALF_SIZE >= limits.second) {
			rotateRight();
			moveForward();
			return 0;
		}

		position += offset;
		model_matrix = glm::mat4();
		model_matrix = glm::translate(model_matrix, position);
		model_matrix = glm::rotate(model_matrix, slope_angle, glm::vec3(1, 0, 0));
		model_matrix = glm::rotate(model_matrix, rotate_angle, glm::vec3(0, 1, 0));
		
		skis_model_matrix = glm::mat4();
		skis_model_matrix = glm::translate(skis_model_matrix, position);
		skis_model_matrix = glm::rotate(skis_model_matrix, slope_angle, glm::vec3(1, 0, 0));
		skis_model_matrix = glm::rotate(skis_model_matrix, rotate_angle, glm::vec3(0, 1, 0));

		fps_cam->set(position, position + glm::vec3(0, 0, 1), glm::vec3(0, 1, 0));
		fps_cam->rotateFPSoZ(-slope_angle);
		fps_cam->rotateFPSoY(rotate_angle);
		fps_cam->translateUpword(HALF_SIZE);
		fps_cam->translateForward(HALF_SIZE);
		
		tps_cam->set(position + tps_cam_offset, position + glm::vec3(0, HALF_SIZE, 0), glm::vec3(0, 1, 0));
		cam_model_matrix = glm::mat4();
		cam_model_matrix = glm::translate(cam_model_matrix, tps_cam->getPosition());
		cam_model_matrix = glm::rotate(cam_model_matrix, -cam_angle, glm::vec3(0, 0, 1));

		// Indrept personajul cand acesta ajunge la baza rampei.
		if (position.y <= Slope::BOTTOM) {
			is_on_slope = false;
			is_in_air = true;
			model_matrix = glm::rotate(model_matrix, -rotate_angle, glm::vec3(0, 1, 0));
			skis_model_matrix = glm::rotate(skis_model_matrix, -rotate_angle, glm::vec3(0, 1, 0));
			fps_cam->rotateFPSoY(-rotate_angle);
			rotate_angle = 0;

			// Personajul nu se poate muta inainte pentru ca patratul pe care
			// trebuie sa aterizeze este ocupat.
			int distance_bottom = (type == MAIN_ACTIVE || type == MAIN_SPECIAL) ? 10 : 11;
			if (quad->isOccupied(ceil(position.x - 1), ceil(position.z + distance_bottom)))
				return 1;
		}
	}
	else if (is_in_air) {
		float angle_speed = speed * slowdown;
		speed = speed * slowdown / 5;
		float y1 = offset.z * offset.z * G * COEF_A + offset.z * COEF_B,
			y2 = (offset.z + speed) * (offset.z + speed) * G * COEF_A + (offset.z + speed) * COEF_B;
		offset.z = offset.z + speed;
		offset.y = y2 - y1;
		position += glm::vec3(0, offset.y, speed);

		model_matrix = glm::mat4();
		model_matrix = glm::translate(model_matrix, position);
		model_matrix = glm::rotate(model_matrix, slope_angle, glm::vec3(1, 0, 0));
		
		skis_model_matrix = glm::mat4();
		skis_model_matrix = glm::translate(skis_model_matrix, position);
		skis_model_matrix = glm::rotate(skis_model_matrix, ski_angle, glm::vec3(1, 0, 0));

		fps_cam->set(position, position + glm::vec3(0, 0, 1), glm::vec3(0, 1, 0));
		fps_cam->rotateFPSoZ(-slope_angle);
		fps_cam->translateUpword(HALF_SIZE);
		fps_cam->translateForward(HALF_SIZE);
		
		tps_cam->set(position + tps_cam_offset, position + glm::vec3(0, HALF_SIZE, 0), glm::vec3(0, 1, 0));
		cam_model_matrix = glm::mat4();
		cam_model_matrix = glm::translate(cam_model_matrix, tps_cam->getPosition());
		cam_model_matrix = glm::rotate(cam_model_matrix, -cam_angle, glm::vec3(0, 0, 1));

		if (y2 > y1) {
			ski_angle += angle_speed;
			skis_model_matrix = glm::rotate(skis_model_matrix, angle_speed, glm::vec3(1, 0, 0));
		}
		else {
			ski_angle -= 2 * angle_speed;
			slope_angle -= 1.5f * angle_speed;
			model_matrix = glm::rotate(model_matrix, - 1.5f * angle_speed, glm::vec3(1, 0, 0));
			skis_model_matrix = glm::rotate(skis_model_matrix, -2 * angle_speed, glm::vec3(1, 0, 0));
			fps_cam->rotateFPSoZ(1.5f * angle_speed);
		}
		if (position.y <= 0) {
			model_matrix = glm::rotate(model_matrix, -slope_angle, glm::vec3(1, 0, 0));
			fps_cam->rotateFPSoZ(slope_angle);
			slope_angle = 0;
			skis_model_matrix = glm::rotate(skis_model_matrix, -ski_angle, glm::vec3(1, 0, 0));
			ski_angle = 0;
		}

		// A ajuns pe quad.
		if (position.y <= 0) {
			model_matrix = glm::mat4();
			skis_model_matrix = glm::mat4();
			position.y = 0;
			model_matrix = glm::translate(model_matrix, position);
			skis_model_matrix = glm::translate(skis_model_matrix, position);
			
			fps_cam->set(position, position + glm::vec3(0, 0, 1), glm::vec3(0, 1, 0));
			fps_cam->translateUpword(HALF_SIZE);
			fps_cam->translateForward(HALF_SIZE);
			
			is_on_quad = true;
			is_in_air = false;

			if (type == SECOND_ACTIVE || type == SECOND_SPECIAL) {
				makePathToFreeSpot();
			}
		}
	}
	else {
		if (type == SECOND_ACTIVE || type == SECOND_SPECIAL) {
			if (!is_inactive)
				moveToFreeSpot();
		}
		else {
			moveOnQuad(1);
		}
	}
	return 0;
}

void Character::moveBack()
{
	moveOnQuad(-1);
}

void Character::moveOnQuad(int move_direction)
{
	
	if (is_on_quad) {
		float sin_angle = sin(rotate_angle * M_PI / 180),
			cos_angle = cos(rotate_angle * M_PI / 180),
			radius = sin(M_PI / 4); // jumatate din diagonala unui patrat cu latura 1.
		offset = glm::vec3(sin_angle * move_direction, 0, cos_angle * move_direction);
		glm::vec3 temp = position + offset, v1, v2, v3, v4;

		v1 = temp + glm::vec3(sin_angle * radius, 0, cos_angle * radius);
		v2 = temp + glm::vec3(sin_angle * radius, 0, -cos_angle * radius);
		v3 = temp + glm::vec3(-sin_angle * radius, 0, -cos_angle * radius);
		v4 = temp + glm::vec3(-sin_angle * radius, 0, cos_angle * radius);

		// Verific toate varfurile.
		if (v1.x < 0 || v1.z < 0 || v1.x > quad->getSize() || v1.z > quad->getSize() || quad->isOccupied(v1.x, v1.z) ||
			v2.x < 0 || v2.z < 0 || v2.x > quad->getSize() || v2.z > quad->getSize() || quad->isOccupied(v2.x, v2.z) ||
			v3.x < 0 || v3.z < 0 || v3.x > quad->getSize() || v3.z > quad->getSize() || quad->isOccupied(v3.x, v3.z) ||
			v4.x < 0 || v4.z < 0 || v4.x > quad->getSize() || v4.z > quad->getSize() || quad->isOccupied(v4.x, v4.z)) {
			offset = glm::vec3(0, 0, 0);
			return;
		}

		position += offset;

		model_matrix = glm::mat4();
		model_matrix = glm::translate(model_matrix, position);
		model_matrix = glm::rotate(model_matrix, rotate_angle, glm::vec3(0, 1, 0));

		skis_model_matrix = glm::mat4();
		skis_model_matrix = glm::translate(skis_model_matrix, position);
		skis_model_matrix = glm::rotate(skis_model_matrix, rotate_angle, glm::vec3(0, 1, 0));

		fps_cam->set(position, position + glm::vec3(0, 0, 1), glm::vec3(0, 1, 0));
		fps_cam->rotateFPSoY(rotate_angle);
		fps_cam->translateUpword(HALF_SIZE);
		fps_cam->translateForward(HALF_SIZE);

		tps_cam->set(position + tps_cam_offset, position + glm::vec3(0, HALF_SIZE, 0), glm::vec3(0, 1, 0));
		cam_model_matrix = glm::mat4();
		cam_model_matrix = glm::translate(cam_model_matrix, tps_cam->getPosition());
		cam_model_matrix = glm::rotate(cam_model_matrix, -cam_angle, glm::vec3(0, 0, 1));
	}
}

void Character::makePathToFreeSpot()
{
	// Aplic un algoritm BFS pe matricea de patrate a quad-ului pentru a
	// determina calea minima catre un patrat liber pe care se poate plasa
	// un personaj secundat. Matricea costs indica distanta de la patratul
	// sursa la oricare alt patrat de pe quad.

	std::vector<std::vector<int>> costs(quad->getSize(), std::vector<int>(quad->getSize(), -1));
	std::pair<int, int> dest = quad->getFirstFree();

	std::pair<int, int> source(ceil(position.x - 1), ceil(position.z - 1));

	std::queue<std::pair<int, int>> queue;
	costs[source.first][source.second] = 0;
	queue.push(source);

	std::pair<int, int> spot;
	while (!queue.empty() && spot != dest) {
		spot = queue.front();
		queue.pop();
		if (spot.first > 0 && costs[spot.first - 1][spot.second] == -1 &&
			!quad->isOccupied(spot.first - 1, spot.second)) {
			costs[spot.first - 1][spot.second] = costs[spot.first][spot.second] + 1;
			queue.push(std::make_pair(spot.first - 1, spot.second));
		}
		if (spot.second > 0 && costs[spot.first][spot.second - 1] == -1 &&
			!quad->isOccupied(spot.first, spot.second - 1)) {
			costs[spot.first][spot.second - 1] = costs[spot.first][spot.second] + 1;
			queue.push(std::make_pair(spot.first, spot.second - 1));
		}
		if (spot.first < quad->getSize() - 1 && costs[spot.first + 1][spot.second] == -1 &&
			!quad->isOccupied(spot.first + 1, spot.second)) {
			costs[spot.first + 1][spot.second] = costs[spot.first][spot.second] + 1;
			queue.push(std::make_pair(spot.first + 1, spot.second));
		}
		if (spot.second < quad->getSize() - 1 && costs[spot.first][spot.second + 1] == -1 &&
			!quad->isOccupied(spot.first, spot.second + 1)) {
			costs[spot.first][spot.second + 1] = costs[spot.first][spot.second] + 1;
			queue.push(std::make_pair(spot.first, spot.second + 1));
		}
		// In caz ca se misca pe diagonala, verific si daca cele doua locuri
		// peste care trece in drum spre destinatie sunt libere.
		if (spot.first > 0 && spot.second > 0 && costs[spot.first - 1][spot.second - 1] == -1 &&
			!quad->isOccupied(spot.first - 1, spot.second - 1) &&
			!quad->isOccupied(spot.first - 1, spot.second) &&
			!quad->isOccupied(spot.first, spot.second - 1)) {
			costs[spot.first - 1][spot.second - 1] = costs[spot.first][spot.second] + 1;
			queue.push(std::make_pair(spot.first - 1, spot.second - 1));
		}
		if (spot.first > 0 && spot.second < quad->getSize() - 1 && costs[spot.first - 1][spot.second + 1] == -1 &&
			!quad->isOccupied(spot.first - 1, spot.second + 1) &&
			!quad->isOccupied(spot.first - 1, spot.second) &&
			!quad->isOccupied(spot.first, spot.second + 1)) {
			costs[spot.first - 1][spot.second + 1] = costs[spot.first][spot.second] + 1;
			queue.push(std::make_pair(spot.first - 1, spot.second + 1));
		}
		if (spot.first < quad->getSize() - 1 && spot.second > 0 && costs[spot.first + 1][spot.second - 1] == -1 &&
			!quad->isOccupied(spot.first + 1, spot.second - 1) &&
			!quad->isOccupied(spot.first + 1, spot.second) &&
			!quad->isOccupied(spot.first, spot.second - 1)) {
			costs[spot.first + 1][spot.second - 1] = costs[spot.first][spot.second] + 1;
			queue.push(std::make_pair(spot.first + 1, spot.second - 1));
		}
		if (spot.first < quad->getSize() - 1 && spot.second < quad->getSize() - 1 &&
			costs[spot.first + 1][spot.second + 1] == -1 &&
			!quad->isOccupied(spot.first + 1, spot.second + 1) &&
			!quad->isOccupied(spot.first + 1, spot.second) &&
			!quad->isOccupied(spot.first, spot.second + 1)) {
			costs[spot.first + 1][spot.second + 1] = costs[spot.first][spot.second] + 1;
			queue.push(std::make_pair(spot.first + 1, spot.second + 1));
		}
	}

	quad->setOccupied(dest.first, dest.second);

	// Formez calea de deplasare mergand inapoi de la destinatie catre sursa.
	spot = dest;
	while (costs[spot.first][spot.second] != 0) {
		if (spot.first > 0 && costs[spot.first - 1][spot.second] == costs[spot.first][spot.second] - 1) {
			spot.first -= 1;
			path.push_back(std::make_pair(1, 0));
		}
		else if (spot.first > 0 && spot.second < quad->getSize() - 1 &&
			costs[spot.first - 1][spot.second + 1] == costs[spot.first][spot.second] - 1) {
			spot.first -= 1;
			spot.second += 1;
			path.push_back(std::make_pair(1, -1));
		}
		else if (spot.second < quad->getSize() - 1 && costs[spot.first][spot.second + 1] == costs[spot.first][spot.second] - 1) {
			spot.second += 1;
			path.push_back(std::make_pair(0, -1));
		}
		else if (spot.first < quad->getSize() - 1 && spot.second < quad->getSize() - 1 &&
			costs[spot.first + 1][spot.second + 1] == costs[spot.first][spot.second] - 1) {
			spot.first += 1;
			spot.second += 1;
			path.push_back(std::make_pair(-1, -1));
		}
		else if (spot.first < quad->getSize() - 1 && costs[spot.first + 1][spot.second] == costs[spot.first][spot.second] - 1) {
			spot.first += 1;
			path.push_back(std::make_pair(-1, 0));
		}
		else if (spot.first < quad->getSize() - 1 && spot.second > 0 &&
			costs[spot.first + 1][spot.second - 1] == costs[spot.first][spot.second] - 1) {
			spot.first += 1;
			spot.second -= 1;
			path.push_back(std::make_pair(-1, 1));
		}
		else if (spot.second > 0 && costs[spot.first][spot.second - 1] == costs[spot.first][spot.second] - 1) {
			spot.second -= 1;
			path.push_back(std::make_pair(0, 1));
		}
		else if (spot.first > 0 && spot.second > 0 &&
			costs[spot.first - 1][spot.second - 1] == costs[spot.first][spot.second] - 1) {
			spot.first -= 1;
			spot.second -= 1;
			path.push_back(std::make_pair(1, 1));
		}
	}
}

void Character::moveToFreeSpot()
{
	float slice = 50;
	static float fraction = 0;
	int rotation;

	if (!path.empty()) {
		// Trebuie sa rotesc personajul in functie de urmatorul patrat de parcurs.
		if (path.back() == std::make_pair(-1, 0)) {
			rotation = -90;
		}
		else if (path.back() == std::make_pair(-1, 1)){
			rotation = -45;
		}
		else if (path.back() == std::make_pair(0, 1)){
			rotation = 0;
		}
		else if (path.back() == std::make_pair(1, 1)){
			rotation = 45;
		}
		else if (path.back() == std::make_pair(1, 0)){
			rotation = 90;
		}
		else if (path.back() == std::make_pair(1, -1)){
			rotation = 135;
		}
		else if (path.back() == std::make_pair(0, -1)){
			rotation = 180;
		}
		else if (path.back() == std::make_pair(-1, -1)){
			rotation = -135;
		}

		glm::vec3 move = glm::vec3(path.back().first / slice, 0, path.back().second / slice);
		position += move;

		model_matrix = glm::mat4();
		model_matrix = glm::translate(model_matrix, position);
		model_matrix = glm::rotate(model_matrix, (float)rotation, glm::vec3(0, 1, 0));
		
		skis_model_matrix = glm::mat4();
		skis_model_matrix = glm::translate(skis_model_matrix, position);
		skis_model_matrix = glm::rotate(skis_model_matrix, (float)rotation, glm::vec3(0, 1, 0));

		fps_cam->set(position, position + glm::vec3(0, 0, 1), glm::vec3(0, 1, 0));
		fps_cam->rotateFPSoY(rotation);
		fps_cam->translateUpword(HALF_SIZE);
		fps_cam->translateForward(HALF_SIZE);

		tps_cam->set(position + tps_cam_offset, position + glm::vec3(0, HALF_SIZE, 0), glm::vec3(0, 1, 0));
		cam_model_matrix = glm::mat4();
		cam_model_matrix = glm::translate(cam_model_matrix, tps_cam->getPosition());
		cam_model_matrix = glm::rotate(cam_model_matrix, -cam_angle, glm::vec3(0, 0, 1));

		// Atunci cand fraction devine mai mare sau egal cu 1 inseamna ca am iterat de slice ori. 
		fraction += 1 / slice;
		if (fraction >= 1) {
			fraction = 0;
			path.pop_back();
		}
	}
	else {
		// Personajul a ajuns la destinatie.
		is_inactive = true;

		model_matrix = glm::mat4();
		skis_model_matrix = glm::mat4();
		model_matrix = glm::translate(model_matrix, position);
		skis_model_matrix = glm::translate(skis_model_matrix, position);
		// Rotesc personajul astfel incat sa fie cu fata catre centrul quad-ului.
		int half = quad->getSize() / 2;
		rotation = -90 - (180 * atan((position.z - half) / half) / M_PI);
		model_matrix = glm::rotate(model_matrix, (float)rotation, glm::vec3(0, 1, 0));
		skis_model_matrix = glm::rotate(skis_model_matrix, (float)rotation, glm::vec3(0, 1, 0));
		
		fps_cam->set(position, position + glm::vec3(0, 0, 1), glm::vec3(0, 1, 0));
		fps_cam->rotateFPSoY(rotation);
		fps_cam->translateUpword(HALF_SIZE);
		fps_cam->translateForward(HALF_SIZE);

		this->updateNormals();
	}
}

bool Character::atDestination()
{
	return is_on_quad;
}

bool Character::onSlope()
{
	return is_on_slope;
}

glm::vec3 Character::getPosition()
{
	return position;
}

void Character::updateNormals()
{
	Util::computeNormals(vertices, indices, model_matrix);
	Util::computeNormals(vertices_skis, indices_skis, skis_model_matrix);
}

lab::Camera * Character::getFpsCam()
{
	return fps_cam;
}

lab::Camera * Character::getTpsCam()
{
	return tps_cam;
}

void Character::setInactive()
{
	is_inactive = true;
}