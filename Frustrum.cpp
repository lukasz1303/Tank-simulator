#include "Frustrum.h"

void Frustrum::setFrustrum(glm::vec3 cameraPos, glm::vec3 cameraFront, glm::vec3 cameraUp, float ratio, float fov, float nearDist, float farDist)
{
	Hnear = 2 * tan(glm::radians(fov / 4.0f)) * nearDist;
	Wnear = Hnear * ratio;

	Hfar = 2 * tan(glm::radians(fov / 4.0f)) * farDist;
	Wfar = Hfar * ratio;
	this->cameraPos = cameraPos;
	this->farDist = farDist;
	this->nearDist = nearDist;
	this->tang = tan(glm::radians(fov / 4.0f));
	this->ratio = ratio;

	sphereFactorY = 1.0 / cos(glm::radians(fov /4.0f));

	// compute half of the the horizontal field of view and sphereFactorX
	float anglex = atan(tang * ratio);
	sphereFactorX = 1.0 / cos(anglex);

	Z = cameraPos - cameraFront;
	Z = glm::normalize(Z);

	X = glm::cross(cameraUp, Z);
	X = glm::normalize(X);

	Y = glm::cross(Z, X);

	nc = cameraPos - Z * nearDist;
	fc = cameraPos - Z * farDist;

	glm::vec3 ntl = nc + Y * Hnear - X * Wnear;
	glm::vec3 ntr = nc + Y * Hnear + X * Wnear;
	glm::vec3 nbl = nc - Y * Hnear - X * Wnear;
	glm::vec3 nbr = nc - Y * Hnear + X * Wnear;

	glm::vec3 ftl = fc + Y * Hfar - X * Wfar;
	glm::vec3 ftr = fc + Y * Hfar + X * Wfar;
	glm::vec3 fbl = fc - Y * Hfar - X * Wfar;
	glm::vec3 fbr = fc - Y * Hfar + X * Wfar;

	planes[TOP] = new Plane(ntr, ntl, ftl);
	planes[BOTTOM] = new Plane(nbl, nbr, fbr);
	planes[LEFT] = new Plane(ntl, nbl, fbl);
	planes[RIGHT] = new Plane(nbr, ntr, fbr);
	planes[NEARP] = new Plane(ntl, ntr, nbr);
	planes[FARP] = new Plane(ftr, ftl, fbl);
}

bool Frustrum::pointInFrustum(glm::vec3 p) {

	bool result = true;

	for (int i = 0; i < 6; i++) {
		if (planes[i]->d + glm::dot(planes[i]->normal, p) < 0) {
			return false;
		}
	}
	//return(result);


	//float pcz, pcx, pcy, aux;

	//// compute vector from camera position to p
	//glm::vec3 v = glm::vec3(p.x - cameraPos.x, p.y - cameraPos.y, p.z - cameraPos.z);

	//// compute and test the Z coordinate
	//pcz = glm::dot(v,-Z);
	//if (pcz > farDist || pcz < nearDist)
	//	return(false);

	//// compute and test the Y coordinate
	//pcy = glm::dot(v, Y);
	//aux = pcz * tang;
	//if (pcy > aux || pcy < -aux)
	//	return(false);

	//// compute and test the X coordinate
	//pcx = glm::dot(v, X);
	//aux = aux * ratio;
	//if (pcx > aux || pcx < -aux)
	//	return(false);

	return(result);
}

bool Frustrum::sphereInFrustum(glm::vec3 p, float radius)
{

	float d;
	float az, ax, ay;
	bool result = true;

	glm::vec3 v = p - cameraPos;

	az = glm::dot(v,-Z);
	if (az > farDist + radius || az < nearDist - radius)
		return(false);

	if (az > farDist - radius || az < nearDist + radius)
		result = true;

	ay = glm::dot(v, Y);
	d = sphereFactorY * radius;
	az *= tang;
	if (ay > az + d || ay < -az - d)
		return(false);

	if (ay > az - d || ay < -az + d)
		result = true;

	ax = glm::dot(v, X);
	az *= ratio;
	d = sphereFactorX * radius;
	if (ax > az + d || ax < -az - d)
		return(false);

	if (ax > az - d || ax < -az + d)
		result = true;

	return(result);
}
