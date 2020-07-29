#pragma once

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

class Floor
{

public:

	float verts[24] = {
		  1000.0f,-1000.0f,0.0f,1.0f, //A
		 -1000.0f, 1000.0f,0.0f,1.0f, //B
		 -1000.0f,-1000.0f,0.0f,1.0f, //C

		  1000.0f,-1000.0f,0.0f,1.0f, //A
		  1000.0f, 1000.0f,0.0f,1.0f, //D
		 -1000.0f, 1000.0f,0.0f,1.0f, //B
	};

	float normals[24] = {
		0.0f, 0.0f,-1.0f,0.0f,
		0.0f, 0.0f,-1.0f,0.0f,
		0.0f, 0.0f,-1.0f,0.0f,

		0.0f, 0.0f,-1.0f,0.0f,
		0.0f, 0.0f,-1.0f,0.0f,
		0.0f, 0.0f,-1.0f,0.0f,
	};


	//Tablica wspólrzednych teksturowania
	float texCoords[12] = {
	  500.0f, 0.0f,   //A
	  0.0f, 500.0f,    //B
	  0.0f, 0.0f,    //C

	  500.0f, 0.0f,    //A
	  500.0f, 500.0f,    //D
	  0.0f, 500.0f,    //B
	};

	float myCubeC1[24] = {
		//Ściana 1
		0.500000f,-0.000000f,0.000000f,0.000000f,
		0.500000f,-0.000000f,0.000000f,0.000000f,
		0.500000f,-0.000000f,0.000000f,0.000000f,
		0.500000f,-0.000000f,0.000000f,0.000000f,
		0.500000f,-0.000000f,0.000000f,0.000000f,
		0.500000f,-0.000000f,0.000000f,0.000000f

	};
	float myCubeC2[24] = {
		//Ściana 1
		-0.000000f,-0.500000f,-0.000000f,0.000000f,
		-0.000000f,-0.500000f,-0.000000f,0.000000f,
		-0.000000f,-0.500000f,-0.000000f,0.000000f,
		-0.000000f,-0.500000f,-0.000000f,0.000000f,
		-0.000000f,-0.500000f,-0.000000f,0.000000f,
		-0.000000f,-0.500000f,-0.000000f,0.000000f

	};
	float myCubeC3[24] = {
		//Ściana 1
		0.000000f,0.000000f,-0.500000f,0.000000f,
		0.000000f,0.000000f,-0.500000f,0.000000f,
		0.000000f,0.000000f,-0.500000f,0.000000f,
		0.000000f,-0.000000f,-0.500000f,0.000000f,
		0.000000f,-0.000000f,-0.500000f,0.000000f,
		0.000000f,-0.000000f,-0.500000f,0.000000f
	};

	//Liczba wierzcholków w tablicy
	int vertexCount = 6;

	Floor();

	void draw_floor(glm::mat4 P, glm::mat4 V, GLuint tex, GLuint tex1, GLuint tex2, ShaderProgram* sp);

};