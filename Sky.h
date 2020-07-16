#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stdlib.h>
#include <stdio.h>
#include "include/shaderprogram.h"
#include "include/Texture.h"

class Sky
{

	float vertices[192] = {
		1.0f,-1.0f,-1.0f,1.0f,
		-1.0f, 1.0f,-1.0f,1.0f,
		-1.0f,-1.0f,-1.0f,1.0f,

		1.0f,-1.0f,-1.0f,1.0f,
		1.0f, 1.0f,-1.0f,1.0f,
		-1.0f, 1.0f,-1.0f,1.0f,


		-1.0f,-1.0f, 1.0f,1.0f,
		1.0f, 1.0f, 1.0f,1.0f,
		1.0f,-1.0f, 1.0f,1.0f,

		-1.0f,-1.0f, 1.0f,1.0f,
		-1.0f, 1.0f, 1.0f,1.0f,
		1.0f, 1.0f, 1.0f,1.0f,

		1.0f,-1.0f, 1.0f,1.0f,
		1.0f, 1.0f,-1.0f,1.0f,
		1.0f,-1.0f,-1.0f,1.0f,

		1.0f,-1.0f, 1.0f,1.0f,
		1.0f, 1.0f, 1.0f,1.0f,
		1.0f, 1.0f,-1.0f,1.0f,

		-1.0f,-1.0f,-1.0f,1.0f,
		-1.0f, 1.0f, 1.0f,1.0f,
		-1.0f,-1.0f, 1.0f,1.0f,

		-1.0f,-1.0f,-1.0f,1.0f,
		-1.0f, 1.0f,-1.0f,1.0f,
		-1.0f, 1.0f, 1.0f,1.0f,

		-1.0f,-1.0f,-1.0f,1.0f,
		1.0f,-1.0f, 1.0f,1.0f,
		1.0f,-1.0f,-1.0f,1.0f,

		-1.0f,-1.0f,-1.0f,1.0f,
		-1.0f,-1.0f, 1.0f,1.0f,
		1.0f,-1.0f, 1.0f,1.0f,

		-1.0f, 1.0f, 1.0f,1.0f,
		1.0f, 1.0f,-1.0f,1.0f,
		1.0f, 1.0f, 1.0f,1.0f,

		-1.0f, 1.0f, 1.0f,1.0f,
		-1.0f, 1.0f,-1.0f,1.0f,
		1.0f, 1.0f,-1.0f,1.0f,

	};


	float texCoords[72] = {
		1.0f,1.0f, 0.0f,0.0f, 0.0f,1.0f,
		1.0f,1.0f, 1.0f,0.0f, 0.0f,0.0f,

		1.0f,1.0f, 0.0f,0.0f, 0.0f,1.0f,
		1.0f,1.0f, 1.0f,0.0f, 0.0f,0.0f,

		1.0f,1.0f, 0.0f,0.0f, 0.0f,1.0f,
		1.0f,1.0f, 1.0f,0.0f, 0.0f,0.0f,

		1.0f,1.0f, 0.0f,0.0f, 0.0f,1.0f,
		1.0f,1.0f, 1.0f,0.0f, 0.0f,0.0f,

		1.0f,1.0f, 0.0f,0.0f, 0.0f,1.0f,
		1.0f,1.0f, 1.0f,0.0f, 0.0f,0.0f,

		1.0f,1.0f, 0.0f,0.0f, 0.0f,1.0f,
		1.0f,1.0f, 1.0f,0.0f, 0.0f,0.0f,
	};



public:
	Sky();

	void draw_sky(glm::mat4 P, glm::mat4 V, Texture skybox[], ShaderProgram* sp, glm::vec3 speed_vector);
};

