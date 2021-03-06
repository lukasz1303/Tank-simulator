#ifndef BOX_H
#define BOX_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stdlib.h>
#include <stdio.h>
#include "constants.h"
#include "lodepng.h"
#include "shaderprogram.h"
#include "include/Floor.h"


class Box
{
private:
	//wspolrzedne obiektu
	glm::vec3 coordinates = glm::vec3(1.0f);
	glm::vec3 box_size = glm::vec3(2.3f, 2.5f, 2.3f);
	glm::vec4 Position = glm::vec4(glm::vec3(0.0f), 1.0f);
	glm::mat4 M_skrzynia = glm::mat4(1.0f);

	//czy zniszczona?
	bool destroyed = false;

	//wektory obiektu
	std::vector< glm::vec4 > vertices;
	std::vector< glm::vec2 > uvs;
	std::vector< glm::vec4 > normals;

public:
	void setObject(std::vector < glm::vec4 > out_vertices, std::vector < glm::vec2 > out_uvs, std::vector < glm::vec4 > out_normals);
	void destroy();
	bool is_destroyed();
	glm::vec3 getPosition();
	glm::vec3 getSize();
	void setCords(glm::vec3 coords);
	void draw(glm::mat4 P, glm::mat4 V, ShaderProgram *sp,Floor &ground, GLuint tex);
};

#endif