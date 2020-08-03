#include "Plane.h"

Plane::Plane()
{
}

Plane::Plane(glm::vec3 v1, glm::vec3 v2, glm::vec3 v3)
{
	glm::vec3 aux1, aux2;
	aux1 = v1 - v2;
	aux2 = v3 - v2;

	glm::vec3 normal = glm::cross(aux2, aux1);
	normal = glm::normalize(normal);
	this->normal = normal;
	this->point = v2;
	this->d = -(glm::dot(normal, v2));
}