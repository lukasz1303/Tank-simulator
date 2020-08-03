#include "Frustrum.h"

void Frustrum::setFrustrum(glm::vec3 cameraPos, glm::vec3 cameraFront, glm::vec3 cameraUp, float ratio, float fov, float nearDist, float farDist)
{
	Hnear = 2 * tan(glm::radians(fov / 4.0f)) * nearDist;
	Wnear = Hnear * ratio;

	Hfar = 2 * tan(glm::radians(fov / 4.0f)) * farDist;
	Wfar = Hfar * ratio;

	glm::vec3 nc, fc, X, Y, Z;

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
	return(result);
}
