#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Plane.h"

class Frustrum
{
private:
	float Hnear, Wnear, Hfar, Wfar;
	glm::vec3 nc, fc, X, Y, Z;
	glm::vec3 ntl, ntr, nbl, nbr, ftl, ftr, fbl, fbr;
	Plane* planes[6];
	enum {
		TOP = 0, BOTTOM, LEFT,
		RIGHT, NEARP, FARP
	};
public:
	void setFrustrum(glm::vec3 cameraPos, glm::vec3 cameraFront, glm::vec3 cameraUp,float ratio, float fov, float nearDist, float farDist);
	bool pointInFrustum(glm::vec3 p);
};

