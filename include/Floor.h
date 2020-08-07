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
	static const int heightsMapSize = 201;

public:

	float verts[960000];

	float normals[960000];


	//Tablica wspólrzednych teksturowania
	float texCoords[480000];

	float *heightMap[heightsMapSize];


	//Liczba wierzcholków w tablicy
	int vertexCount = 6;

	Floor();

	void draw_floor(glm::mat4 P, glm::mat4 V, GLuint tex, GLuint tex1, GLuint tex2, ShaderProgram* sp, glm::vec3 cameraPos);
	void createHeightMapArray();
	float calculateHeight(float X, float Z);

};