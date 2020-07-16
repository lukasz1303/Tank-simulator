#ifndef TREE_H
#define TREE_H

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

class Tree
{
private:
	//wspolrzedne obiektu
	glm::vec3 coordinates;
	float scale = 2.0f;

	//wektory
	std::vector< glm::vec4 > vertices;
	std::vector< glm::vec2 > uvs;
	std::vector< glm::vec4 > normals;

public:
	void setObject(std::vector < glm::vec4 > out_vertices, std::vector < glm::vec2 > out_uvs, std::vector < glm::vec4 > out_normals);
	void setCords(glm::vec3 coords);
	void draw(glm::mat4 P, glm::mat4 V, ShaderProgram* sp, GLuint tex, GLuint tex2);
	void setScale(float scale);
};

#endif