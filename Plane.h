#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <algorithm>
#include <fstream>
#include <istream>
#include <string>
#include <iostream>

class Plane
{
public:
	glm::vec3 normal;
	glm::vec3 point;
	float d;

	Plane();
	Plane(glm::vec3 v1, glm::vec3 v2, glm::vec3 v3);

};

