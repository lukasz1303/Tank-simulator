/*
Niniejszy program jest wolnym oprogramowaniem; możesz go
rozprowadzać dalej i / lub modyfikować na warunkach Powszechnej
Licencji Publicznej GNU, wydanej przez Fundację Wolnego
Oprogramowania - według wersji 2 tej Licencji lub(według twojego
wyboru) którejś z późniejszych wersji.
Niniejszy program rozpowszechniany jest z nadzieją, iż będzie on
użyteczny - jednak BEZ JAKIEJKOLWIEK GWARANCJI, nawet domyślnej
gwarancji PRZYDATNOŚCI HANDLOWEJ albo PRZYDATNOŚCI DO OKREŚLONYCH
ZASTOSOWAŃ.W celu uzyskania bliższych informacji sięgnij do
Powszechnej Licencji Publicznej GNU.
Z pewnością wraz z niniejszym programem otrzymałeś też egzemplarz
Powszechnej Licencji Publicznej GNU(GNU General Public License);
jeśli nie - napisz do Free Software Foundation, Inc., 59 Temple
Place, Fifth Floor, Boston, MA  02110 - 1301  USA
*/

#define GLM_FORCE_RADIANS

#include <algorithm>
#include <fstream>
#include <istream>
#include <string>
#include <iostream>
#include <irrKlang-1.6.0/include/irrKlang.h>


#include "include/Tree.h"
#include "include/Bullet.h"
#include "include/Tank.h"
#include "include/Box.h"
#include "include/Floor.h"
#include "include/Texture.h"
#include "include/Lantern.h"
#include "include/shaderprogram.h"
#include "include/ParticleSystem.h"
#include "Sky.h"
#include "ObjectLoader.h"
#include "Grass.h"


Bullet bullet = Bullet();
Tank tank = Tank();
Box box = Box();
Tree tree = Tree();
Tree tree2 = Tree();
Tree* trees[2500];
Lantern lantern = Lantern();
Lantern lantern2 = Lantern();
Texture floor_texture = Texture();
Texture floor_texture1 = Texture();
Texture floor_texture2 = Texture();
Texture lamp_bottom_texture = Texture();
Texture lamp_white_texture = Texture();
Texture box_texture = Texture();
Texture tree_texture = Texture();
Texture tree_texture2 = Texture();
Texture bullet_texture = Texture();
Texture wheel_texture = Texture();
Texture tank_texture = Texture();
Texture smog_texture = Texture();
Texture grass_texture = Texture();

Texture skybox[6] = { Texture(), Texture(), Texture(), Texture(), Texture(), Texture() };
Floor ground = Floor();
Sky sky = Sky();
ParticleSystem particleSystem = ParticleSystem();
ObjectLoader loader = ObjectLoader();
Grass grass = Grass();

struct Plane {
	glm::vec3 normal;
	glm::vec3 point;
	float d;
};

Plane pl[6];

enum {
	TOP = 0, BOTTOM, LEFT,
	RIGHT, NEARP, FARP
};
irrklang::ISoundEngine* SoundEngine = irrklang::createIrrKlangDevice();


float angle = 90.0f;
bool shoot_ball = false;
bool fisrt_frame_shot = true;
float lastX = 960;
float lastY = 540;
float yaw = 0.0f;
float pitch = 0.0f;
float yaw_limit_down = 0.0f;
float yaw_limit_up = 10.0f;
float wheel_speed_right = 0.0f;
float wheel_speed_left = 0.0f;
float fov = 100.0f;
float nearDist = 1.0f;
float farDist = 100.0f;
float ratio = 1920.0f / 1080.0f;

const float movingSpeed = 0.1f;
const float rotateSpeed = PI / 2;

bool w_press = false;
bool s_press = false;
bool a_press = false;
bool d_press = false;
bool space_press = false;
bool shot_audio = false;
bool zoom = false;

glm::vec3 cameraPos = glm::vec3(0.0f, 3.0f, 7.5f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
glm::vec4 tank_position = glm::vec4(0.0f, 0.0f, -1.0f, 0.0f);
glm::vec3 speed_vector = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 camera_transform = glm::vec3(0.0f, 3.0f, 7.5f);

std::vector< glm::vec4 > vertices;
std::vector< glm::vec2 > uvs;
std::vector< glm::vec4 > normals;
int numberOfTextures = 0;
std::vector<int> startVertices;
std::vector<GLuint> texes;


ShaderProgram* spt;
ShaderProgram* spp;
ShaderProgram* spg;
ShaderProgram* spl;
ShaderProgram* sptree;
ShaderProgram* spgrass;

void readAllTextures();
void loadAllObjects();
void loadShaders();
void freeOpenGLProgram(GLFWwindow* window);
bool pointInFrustum(glm::vec3 p);

void set3Points(glm::vec3 v1, glm::vec3 v2, glm::vec3 v3) {


	glm::vec3 aux1, aux2;

	aux1 = v1 - v2;
	aux2 = v3 - v2;

	glm::vec3 normal = aux2 * aux1;

	normal = glm::normalize(normal);
	glm::vec3 point = v2;
	float d = -(glm::dot(normal,point));
}

//Procedura obsługi myszki
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	float xoffset = lastX - xpos;
	float yoffset = lastY - ypos;
	lastX = xpos;
	lastY = ypos;

	const float sensitivity = 0.1f;
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	yaw += yoffset;
	pitch += xoffset;

	if (yaw > yaw_limit_up)
		yaw = yaw_limit_up;
	if (yaw < yaw_limit_down)
		yaw = yaw_limit_down;

	glm::vec3 direction;
	direction.x = camera_transform[0] * cos(glm::radians(xoffset)) + camera_transform[2] * sin(glm::radians(xoffset));
	direction.z = -camera_transform[0] * sin(glm::radians(xoffset)) + camera_transform[2] * cos(glm::radians(xoffset));
	direction.y = camera_transform.y;
	camera_transform = direction;
}

//Procedura obsługi klawiatury
void key_callback(GLFWwindow* window, int key,
	int scancode, int action, int mods) {
	const float cameraSpeed = 0.001f;

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		w_press = true;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_RELEASE)
		w_press = false;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		s_press = true;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_RELEASE)
		s_press = false;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		a_press = true;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_RELEASE)
		a_press = false;
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		d_press = true;
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_RELEASE)
		d_press = false;
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
		space_press = true;
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_RELEASE)
		space_press = false;

	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		freeOpenGLProgram(window);

		glfwDestroyWindow(window);
		glfwTerminate();
		exit(EXIT_SUCCESS);
	}
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) {
		zoom = !zoom;
		if (zoom) {
			camera_transform = glm::vec3(camera_transform.x - sin(glm::radians(pitch))*(2), camera_transform.y-0.2f, camera_transform.z - cos(glm::radians(pitch)) * (2));
		}
		else {
			camera_transform = glm::vec3(camera_transform.x + sin(glm::radians(pitch)) * (2), camera_transform.y+0.2f, camera_transform.z + cos(glm::radians(pitch)) * (2));
		}
	}
}



//Procedura obsługi błędów
void error_callback(int error, const char* description) {
	fputs(description, stderr);
}

//Procedura inicjująca
void initOpenGLProgram(GLFWwindow* window) {

	glClearColor(0.3, 0.8, 1, 1);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	
	readAllTextures();
	loadAllObjects();
	loadShaders();

	glfwSetKeyCallback(window, key_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	
}


//Zwolnienie zasobów zajętych przez program
void freeOpenGLProgram(GLFWwindow* window) {
	glDeleteTextures(1, &floor_texture.tex);
	glDeleteTextures(1, &lamp_bottom_texture.tex);
	glDeleteTextures(1, &lamp_white_texture.tex);
	glDeleteTextures(1, &box_texture.tex);
	glDeleteTextures(1, &tree_texture.tex);
	glDeleteTextures(1, &tree_texture2.tex);
	glDeleteTextures(1, &bullet_texture.tex);
	glDeleteTextures(1, &wheel_texture.tex);
	glDeleteTextures(1, &tank_texture.tex);
}

//Procedura rysująca zawartość sceny
void drawScene(GLFWwindow* window) {

	glm::vec3 speed = glm::vec3(0.0f);
	printf("%f\n", pitch);
	if (w_press) {
		speed = glm::vec3(-movingSpeed * cos(angle * PI / 180), 0.0f, movingSpeed * sin(angle * PI / 180));
		speed_vector -= speed;
		wheel_speed_left += movingSpeed;
		wheel_speed_right += movingSpeed;
	}
	if (s_press) {
		speed = glm::vec3(movingSpeed * cos(angle * PI / 180), 0.0f, -movingSpeed * sin(angle * PI / 180));
		speed_vector -= speed;
		wheel_speed_left -= movingSpeed;
		wheel_speed_right -= movingSpeed;
	}
	if (a_press) {
		angle += rotateSpeed;
		wheel_speed_right += movingSpeed * 0.5f;
		wheel_speed_left -= movingSpeed * 0.5f;
	}
	if (d_press) {
		angle -= rotateSpeed;
		wheel_speed_right -= movingSpeed * 0.5f;
		wheel_speed_left += movingSpeed * 0.5f;
	}
	if (space_press) {
		shoot_ball = true;
	}
	tank_position = tank.getPosition();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//glm::mat4 V = glm::lookAt(cameraPos, cameraFront, cameraUp);
	glm::mat4 P = glm::perspective(glm::radians(fov/2.0f), ratio, nearDist, farDist);

	spt->use();

	glUniform4f(spt->u("lp"), -4, 3.5, -4, 1);
	glUniform4f(spt->u("lp2"), -50, 20, -50, 1);

	tank.move(P,speed_vector, wheel_speed_left, wheel_speed_right, angle, pitch, yaw, camera_transform, cameraFront, cameraPos, cameraUp, spt, tank_texture.tex, wheel_texture.tex);
	glm::mat4 V = glm::lookAt(cameraPos, cameraFront, cameraUp);



	float Hnear = 2 * tan(glm::radians(fov / 2.0f)) * nearDist;
	float Wnear = Hnear * ratio;

	float Hfar = 2 * tan(glm::radians(fov / 2.0f)) * farDist;
	float Wfar = Hfar * ratio;

	glm::vec3 p = cameraPos;
	//glm::vec3 d = glm::normalize(cameraFront);
	//glm::vec3 up = cameraUp;

	//glm::mat4 rotationMat(1);
	//rotationMat = glm::rotate(rotationMat, -90.0f, glm::vec3(0.0, 1.0, 0.0));
	//glm::vec3 right = glm::vec3(rotationMat * glm::vec4(d, 1.0));


	glm::vec3 dir, nc, fc, X, Y, Z;

	// compute the Z axis of camera
	// this axis points in the opposite direction from
	// the looking direction
	Z = p - cameraFront;
	Z = glm::normalize(Z);

	// X axis of camera with given "up" vector and Z axis
	X = glm::cross(cameraUp, Z);
	X = glm::normalize(X);

	// the real "up" vector is the cross product of Z and X
	Y = glm::cross(Z,X);

	// compute the centers of the near and far planes
	nc = p - Z * nearDist;
	fc = p - Z * farDist;
	//printf("wnear = %f\n", Wnear);
	//printf("Hnear = %f\n", Hnear);
	//printf("wnear = %f\n", Wfar);
	//printf("Hnear = %f\n", Hfar);
	//printf("cameraPos = %f  %f  %f\n", cameraPos.x, cameraPos.y, cameraPos.z);
	//printf("cameraTransform = %f  %f  %f\n", camera_transform.x, camera_transform.y, camera_transform.z);
	//printf("nc = %f  %f  %f\n", nc.x, nc.y, nc.z);
	//printf("fc = %f  %f  %f\n", fc.x, fc.y, fc.z);
	//printf("Y = %f  %f  %f\n", Y.x, Y.y, Y.z);
	// compute the 4 corners of the frustum on the near plane
	glm::vec3 ntl = nc + Y * Hnear - X * Wnear;
	glm::vec3 ntr = nc + Y * Hnear + X * Wnear;
	glm::vec3 nbl = nc - Y * Hnear - X * Wnear;
	glm::vec3 nbr = nc - Y * Hnear + X * Wnear;

	// compute the 4 corners of the frustum on the far plane
	glm::vec3 ftl = fc + Y * Hfar - X * Wfar;
	glm::vec3 ftr = fc + Y * Hfar + X * Wfar;
	glm::vec3 fbl = fc - Y * Hfar - X * Wfar;
	glm::vec3 fbr = fc - Y * Hfar + X * Wfar;

	//printf("ntl = %f,%f,%f\n", ntl.x,ntl.t,ntl.z);
	//printf("ntr = %f,%f,%f\n", ntr.x, ntr.t, ntr.z);
	//printf("nbl = %f,%f,%f\n", nbl.x, nbl.t, nbl.z);
	//printf("nbr = %f,%f,%f\n", nbr.x, nbr.t, nbr.z);
	//printf("ftl = %f,%f,%f\n", ftl.x, ftl.t, ftl.z);
	//printf("ftr = %f,%f,%f\n", ftr.x, ftr.t, ftr.z);
	//printf("fbl = %f,%f,%f\n", fbl.x, fbl.t, fbl.z);
	//printf("fbr = %f,%f,%f\n", fbr.x, fbr.t, fbr.z);

	// compute the six planes
	// the function set3Points assumes that the points
	// are given in counter clockwise order
	glm::vec3 v1 = ntr;
	glm::vec3 v2 = ntl;
	glm::vec3 v3 = ftl;

	glm::vec3 aux1, aux2;

	aux1 = v1 - v2;
	aux2 = v3 - v2;

	glm::vec3 normal = glm::cross(aux2, aux1);

	normal = glm::normalize(normal);
	glm::vec3 point = v2;
	float d2 = -(glm::dot(normal, point));

	pl[TOP].normal = normal;
	pl[TOP].point = point;
	pl[TOP].d = d2;

	v1 = nbl;
	v2 = nbr;
	v3 = fbr;

	aux1 = v1 - v2;
	aux2 = v3 - v2;

	normal = glm::cross(aux2, aux1);

	normal = glm::normalize(normal);
	point = v2;
	d2 = -(glm::dot(normal, point));

	pl[BOTTOM].normal = normal;
	pl[BOTTOM].point = point;
	pl[BOTTOM].d = d2;

	v1 = ntl;
	v2 = nbl;
	v3 = fbl;

	aux1 = v1 - v2;
	aux2 = v3 - v2;

	normal = glm::cross(aux2, aux1);

	normal = glm::normalize(normal);
	point = v2;
	d2 = -(glm::dot(normal, point));

	pl[LEFT].normal = normal;
	pl[LEFT].point = point;
	pl[LEFT].d = d2;

	v1 = nbr;
	v2 = ntr;
	v3 = fbr;

	aux1 = v1 - v2;
	aux2 = v3 - v2;

	normal = glm::cross(aux2, aux1);

	normal = glm::normalize(normal);
	point = v2;
	d2 = -(glm::dot(normal, point));

	pl[RIGHT].normal = normal;
	pl[RIGHT].point = point;
	pl[RIGHT].d = d2;

	v1 = ntl;
	v2 = ntr;
	v3 = nbr;

	aux1 = v1 - v2;
	aux2 = v3 - v2;

	normal = glm::cross(aux2, aux1);

	normal = glm::normalize(normal);
	point = v2;
	d2 = -(glm::dot(normal, point));

	pl[NEARP].normal = normal;
	pl[NEARP].point = point;
	pl[NEARP].d = d2;

	v1 = ftr;
	v2 = ftl;
	v3 = fbl;

	aux1 = v1 - v2;
	aux2 = v3 - v2;

	normal = glm::cross(aux2, aux1);

	normal = glm::normalize(normal);
	point = v2;
	d2 = -(glm::dot(normal, point));

	pl[FARP].normal = normal;
	pl[FARP].point = point;
	pl[FARP].d = d2;

	/*for (int i = 0; i < 6; i++) {
		printf("%f, %f, %f, %f\n", pl[i].normal.x, pl[i].normal.y, pl[i].normal.z, pl[i].d);
	}*/

	sky.draw_sky(P, V, skybox, spl, speed_vector);

	glm::vec3 ppp = tree.getCords();
	//if (pointInFrustum(ppp))
	//	tree.draw(P, V, sptree, tree_texture.tex, tree_texture2.tex);
	//tree2.draw(P, V, sptree, tree_texture.tex, tree_texture2.tex);
	int count = 0;
	for (int i = 0; i < 2500; i++) {
		//if (pointInFrustum(trees[i]->getCords())) {
			trees[i]->draw(P, V, sptree, tree_texture.tex, tree_texture2.tex);
			count++;
		//}
			
		//else
			//printf("outside %d\n", i);
	}
	printf("count = %d\n", count);
	ground.draw_floor(P, V, floor_texture.tex, floor_texture1.tex, floor_texture2.tex, spg);

	grass.draw(P, V, spgrass, tank.getPosition(), grass_texture.tex, floor_texture.tex);

	shoot_ball = bullet.shooting(shoot_ball);

	lantern.draw(P, V, spt,spl, lamp_bottom_texture.tex,lamp_white_texture.tex);
	lantern2.draw(P, V, spt,spl, lamp_bottom_texture.tex, lamp_white_texture.tex);


	if (!bullet.hasCollision(box.getPosition(), box.getSize(), box.is_destroyed()))
	{
		box.draw(P, V, spt, box_texture.tex);
	}
	else if (box.is_destroyed() == false)
	{
		box.destroy();
	}

	if (shoot_ball == true)
	{
		bullet.generate(P, V, tank.getM_lufa(), spt, bullet_texture.tex, particleSystem, SoundEngine);
		particleSystem.drawParticles(P, V, spp, tank.getM_lufa(), cameraPos, pitch, angle, speed,smog_texture.tex);
	}

	glfwSwapBuffers(window);
}


int main(void)
{
	GLFWwindow* window;

	glfwSetErrorCallback(error_callback);

	if (!glfwInit()) 
	{ 
		fprintf(stderr, "Nie można zainicjować GLFW.\n");
		exit(EXIT_FAILURE);
	}

	window = glfwCreateWindow(1920, 1080, "OpenGL", NULL, NULL);
	//window = glfwCreateWindow(1920, 1080, "OpenGL", glfwGetPrimaryMonitor(), NULL);

	if (!window)
	{
		fprintf(stderr, "Nie można utworzyć okna.\n");
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwMakeContextCurrent(window);
	glfwSwapInterval(0);

	if (glewInit() != GLEW_OK) 
	{ 
		fprintf(stderr, "Nie można zainicjować GLEW.\n");
		exit(EXIT_FAILURE);
	}

	initOpenGLProgram(window);


	//Główna pętla
	while (!glfwWindowShouldClose(window))
	{
		drawScene(window);
		glfwPollEvents();
	}

	freeOpenGLProgram(window);

	glfwDestroyWindow(window);
	glfwTerminate();
	exit(EXIT_SUCCESS);
}

void readAllTextures() {
	floor_texture.readTexture((char*)"textures/Ground_Forest.png");
	printf("Loaded ground.png\n");

	floor_texture1.readTexture((char*)"textures/Ground_Forest_normal.png");
	printf("Loaded ground_normal.png\n");

	floor_texture2.readTexture((char*)"textures/Ground_Forest_height.png");
	printf("Loaded ground_height.png\n");

	lamp_bottom_texture.readTexture((char*)"textures/lantern_botom.png");
	printf("Loaded lantern_botom.png\n");

	lamp_white_texture.readTexture((char*)"textures/lamp.png");
	printf("Loaded lamp.png\n");

	box_texture.readTexture((char*)"textures/light_wood.png");
	printf("Loaded light_wood.png\n");

	tree_texture.readTexture((char*)"textures/BarkPine2.png");
	printf("Loaded leaf.png\n");

	tree_texture2.readTexture((char*)"textures/Fantasy_conifer_3.png");
	printf("Loaded tree.png\n");

	bullet_texture.readTexture((char*)"textures/bullet.png");
	printf("Loaded bullet.png\n");

	wheel_texture.readTexture((char*)"textures/wheel.png");
	printf("Loaded wheel.png\n");

	tank_texture.readTexture((char*)"textures/tank.png");
	printf("Loaded tank.png\n");

	smog_texture.readTexture((char*)"textures/smog.png");
	printf("Loaded smog.png\n");

	grass_texture.readTexture((char*)"textures/grass2.png");
	printf("Loaded grass.png\n");

	char carray[6][128] =
	{
			"textures/front.png",
			"textures/back.png",
			"textures/right.png",
			"textures/left.png",
			"textures/bottom.png",
			"textures/top.png",
	};

	for (int i = 0; i < 6; i++) {
		std::cout << carray[i] << std::endl;
		skybox[i].readTexture((char*)carray[i]);
	}
}

void loadAllObjects() {
	bool res = loader.loadOBJ("objects/bullet.obj", vertices, uvs, normals);
	printf("Loaded bullet.obj %d\n", res);
	bullet.setObject(vertices, uvs, normals);

	res = loader.loadOBJ("objects/box.obj", vertices, uvs, normals);
	printf("Loaded box.obj %d\n", res);
	box.setObject(vertices, uvs, normals);

	res = loader.loadOBJ("objects/lampa_bottom.obj", vertices, uvs, normals);
	printf("Loaded lampa_bottom.obj %d\n", res);
	lantern.setBottomObject(vertices, uvs, normals);
	lantern2.setBottomObject(vertices, uvs, normals);

	res = loader.loadOBJ("objects/lamp.obj", vertices, uvs, normals);
	printf("Loaded lamp.obj %d\n", res);

	lantern.setLampObject(vertices, uvs, normals);
	lantern2.setLampObject(vertices, uvs, normals);

	res = loader.loadOBJ("objects/bottom.obj", vertices, uvs, normals);
	printf("Loaded bottom.obj %d\n", res);
	tank.setObjectBottom(vertices, uvs, normals);

	res = loader.loadOBJ("objects/turret.obj", vertices, uvs, normals);
	printf("Loaded turret.obj %d\n", res);
	tank.setObjectTurret(vertices, uvs, normals);

	res = loader.loadOBJ("objects/lufa.obj", vertices, uvs, normals);
	printf("Loaded lufa.obj %d\n", res);
	tank.setObjectBarrel(vertices, uvs, normals);

	res = loader.loadOBJ("objects/wheel.obj", vertices, uvs, normals);
	printf("Loaded wheel.obj %d\n", res);
	tank.setObjectWheel(vertices, uvs, normals);

	res = loader.loadOBJ("objects/tree5.obj", vertices, uvs, normals, numberOfTextures, startVertices);
	printf("Loaded tree.obj %d\n",res);
	texes.clear();
	texes.push_back(tree_texture.tex);
	texes.push_back(tree_texture2.tex);
	texes.push_back(tree_texture2.tex);
	texes.push_back(tree_texture2.tex);

	printf("%d\n\n\n", numberOfTextures);
	tree.setObject(vertices, uvs, normals, numberOfTextures, startVertices, texes);
	tree2.setObject(vertices, uvs, normals, numberOfTextures, startVertices, texes);

	glm::vec3 cords[2500];
	int n = 0;
	for (int i = 0; i < 50; i++) {
		for (int j = 0; j < 50; j++) {
			float x = (float)(rand() % 20 - 500 + i * 20);
			float z = (float)(rand() % 20 - 500 + j * 20);
			if (x < 10 && z <10 && z >-10 && x > -10) {
				x = 25.0f;
				z = 25.0f;
			}
			cords[n] = glm::vec3(x, 0.0f, z);
			n += 1;
		}
	}

	for (int i = 0; i < 2500; i++) {

		trees[i] = new Tree();
		trees[i]->setObject(vertices, uvs, normals, numberOfTextures, startVertices, texes);
		trees[i]->setCords(cords[i]);
		trees[i]->setScale((float)(rand() % 200 + 100) / 100.0f);

	}

	tree.setCords(glm::vec3(2.0f, 0.0f, -20.0f));
	tree2.setCords(glm::vec3(-17.0f, 0.0f, 10.0f));

	box.setCords(glm::vec3(4.0f, 0.0f, -4.0f));

	lantern.setCords(glm::vec3(-4.0f, 0.0f, -4.0f));
	lantern2.setCords(glm::vec3(-12.0f, 0.0f, -12.0f));
}

void loadShaders() {
	spt = new ShaderProgram("shaders/v_text.glsl", NULL, "shaders/f_text.glsl");
	spp = new ShaderProgram("shaders/v_particle_text.glsl", NULL, "shaders/f_particle_text.glsl");
	spg = new ShaderProgram("shaders/v_floor.glsl", NULL, "shaders/f_floor.glsl");
	spl = new ShaderProgram("shaders/v_lamp.glsl", NULL, "shaders/f_lamp.glsl");
	sptree = new ShaderProgram("shaders/v_tree.glsl", NULL, "shaders/f_tree.glsl");
	spgrass = new ShaderProgram("shaders/v_grass.glsl", NULL, "shaders/f_grass.glsl");
}

bool pointInFrustum(glm::vec3 p) {

	bool result = true;

	for (int i = 0; i < 6; i++) {
		if (i == 2 || i ==3) {
			continue;
		}
		if (pl[i].d + glm::dot(pl[i].normal, p) < 0) {
			//printf("%d\n", i);
			return false;
		}
	}
	return(result);
}