#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include "include\constants.h"
#include "include\lodepng.h"
#include "include\shaderprogram.h"


class Grass
{
private:
	float verts[24] = {
	  1.0f,-1.0f,0.0f,1.0f, //A
	 -1.0f, 1.0f,0.0f,1.0f, //B
	 -1.0f,-1.0f,0.0f,1.0f, //C

	  1.0f,-1.0f,0.0f,1.0f, //A
	  1.0f, 1.0f,0.0f,1.0f, //D
	 -1.0f, 1.0f,0.0f,1.0f, //B
	};

	float texCoords[12] = {
	  1.0f, 0.0f,   //A
	  0.0f, 1.0f,    //B
	  0.0f, 0.0f,    //C

	  1.0f, 0.0f,    //A
	  1.0f, 1.0f,    //D
	  0.0f, 1.0f,    //B
	};

public:
	void draw(glm::mat4 P, glm::mat4 V, ShaderProgram* sp, GLuint tex);
};

