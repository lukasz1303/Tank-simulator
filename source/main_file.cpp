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


Bullet bullet = Bullet();
Tank tank = Tank();
Box box = Box();
Tree tree = Tree();
Tree tree2 = Tree();
Tree* trees[100];
Lantern lantern = Lantern();
Lantern lantern2 = Lantern();
Texture floor_texture = Texture();
Texture lamp_bottom_texture = Texture();
Texture lamp_white_texture = Texture();
Texture box_texture = Texture();
Texture tree_texture = Texture();
Texture tree_texture2 = Texture();
Texture bullet_texture = Texture();
Texture wheel_texture = Texture();
Texture tank_texture = Texture();
Texture smog_texture1 = Texture();
Texture smog_texture2 = Texture();
Texture smog_texture3 = Texture();
Texture smog_texture4 = Texture();
Texture smog_texture5 = Texture();
Texture smog_texture6 = Texture();
Texture smog_texture7 = Texture();
Texture smog_texture8 = Texture();

Texture skybox[6] = { Texture(), Texture(), Texture(), Texture(), Texture(), Texture() };
Floor ground = Floor();
Sky sky = Sky();
ParticleSystem particleSystem = ParticleSystem();
ObjectLoader loader = ObjectLoader();


float angle = 90.0f;
bool shoot_ball = false;
bool fisrt_frame_shot = true;
float lastX = 400;
float lastY = 300;
float yaw = 0.0f;
float pitch = 0.0f;
float yaw_limit_down = 0.0f;
float yaw_limit_up = 10.0f;
float wheel_speed_right = 0.0f;
float wheel_speed_left = 0.0f;

const float movingSpeed = 0.1f;
const float rotateSpeed = PI / 2;

bool w_press = false;
bool s_press = false;
bool a_press = false;
bool d_press = false;

glm::vec3 cameraPos = glm::vec3(0.0f, 2.6f, 6.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
glm::vec4 tank_position = glm::vec4(0.0f, 0.0f, -1.0f, 0.0f);
glm::vec3 speed_vector = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 camera_transform = glm::vec3(0.0f, 2.6f, 6.0f);

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

void readAllTextures();
void loadAllObjects();
void loadShaders();
void freeOpenGLProgram(GLFWwindow* window);

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
	direction.y = 2.6f;
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
		shoot_ball = true;
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		freeOpenGLProgram(window);

		glfwDestroyWindow(window);
		glfwTerminate();
		exit(EXIT_SUCCESS);
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
	glfwSetCursorPosCallback(window, mouse_callback);

	glfwSetKeyCallback(window, key_callback);

	readAllTextures();
	loadAllObjects();
	loadShaders();
	particleSystem.initializeSystem(2000);
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
	if (w_press) {
		speed_vector.z -= movingSpeed * sin(angle * PI / 180);
		speed_vector.x += movingSpeed * cos(angle * PI / 180);
		wheel_speed_left += movingSpeed;
		wheel_speed_right += movingSpeed;
	}
	if (s_press) {
		speed_vector.z += movingSpeed * sin(angle * PI / 180);
		speed_vector.x -= movingSpeed * cos(angle * PI / 180);
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
	tank_position = tank.getPosition();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glm::mat4 V = glm::lookAt(cameraPos, cameraFront, cameraUp);
	//std::cout << "x " << cameraPos.x << ", y " << cameraPos.y << ", z " << cameraPos.z << std::endl;
	glm::mat4 P = glm::perspective(glm::radians(85.0f), 1.8f, 1.0f, 100.0f);

	sky.draw_sky(P, V, skybox, spl, speed_vector);

	spt->use();

	glUniform4f(spt->u("lp"), -4, 3.5, -4, 1);
	glUniform4f(spt->u("lp2"), -50, 20, -50, 1);

	tank.move(P,speed_vector, wheel_speed_left, wheel_speed_right, angle, pitch, yaw, camera_transform, cameraFront, cameraPos, cameraUp, spt, tank_texture.tex, wheel_texture.tex);

	tree.draw(P, V, sptree, tree_texture.tex, tree_texture2.tex);
	tree2.draw(P, V, sptree, tree_texture.tex, tree_texture2.tex);

	for (int i = 0; i < 25; i++) {
		trees[i]->draw(P, V, sptree, tree_texture.tex, tree_texture2.tex);
	}
	ground.draw_floor(P, V, floor_texture.tex, spg);



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
		particleSystem.drawParticles(P, V, spp, tank.getM_lufa(), cameraPos, pitch, angle, smog_texture1.tex, smog_texture2.tex, smog_texture3.tex, smog_texture4.tex, smog_texture5.tex, smog_texture6.tex, smog_texture7.tex, smog_texture8.tex);
		bullet.generate(P, V, tank.getM_lufa(), spt, bullet_texture.tex, particleSystem);
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
	glfwSwapInterval(1);

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
	floor_texture.readTexture((char*)"textures/ground.png");
	printf("Loaded ground.png\n");

	lamp_bottom_texture.readTexture((char*)"textures/lantern_botom.png");
	printf("Loaded lantern_botom.png\n");

	lamp_white_texture.readTexture((char*)"textures/lamp.png");
	printf("Loaded lamp.png\n");

	box_texture.readTexture((char*)"textures/light_wood.png");
	printf("Loaded light_wood.png\n");

	tree_texture.readTexture((char*)"textures/tree.png");
	printf("Loaded tree.png\n");

	tree_texture2.readTexture((char*)"textures/leaf.png");
	printf("Loaded leaf.png\n");

	bullet_texture.readTexture((char*)"textures/bullet.png");
	printf("Loaded bullet.png\n");

	wheel_texture.readTexture((char*)"textures/wheel.png");
	printf("Loaded wheel.png\n");

	tank_texture.readTexture((char*)"textures/tank.png");
	printf("Loaded tank.png\n");

	smog_texture1.readTexture((char*)"textures/smog.png");
	printf("Loaded smog.png\n");
	smog_texture2.readTexture((char*)"textures/smog2.png");
	printf("Loaded smog2.png\n");
	smog_texture3.readTexture((char*)"textures/smog3.png");
	printf("Loaded smog3.png\n");
	smog_texture4.readTexture((char*)"textures/smog4.png");
	printf("Loaded smog4.png\n");
	smog_texture5.readTexture((char*)"textures/smog5.png");
	printf("Loaded smog5.png\n");
	smog_texture6.readTexture((char*)"textures/smog6.png");
	printf("Loaded smog6.png\n");
	smog_texture7.readTexture((char*)"textures/smog7.png");
	printf("Loaded smog7.png\n");
	smog_texture8.readTexture((char*)"textures/smog8.png");
	printf("Loaded smog8.png\n");
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

	res = loader.loadOBJ("objects/tree2.obj", vertices, uvs, normals, numberOfTextures, startVertices);
	printf("Loaded tree.obj %d\n",res);
	texes.clear();
	texes.push_back(tree_texture.tex);
	texes.push_back(tree_texture2.tex);

	tree.setObject(vertices, uvs, normals, numberOfTextures, startVertices, texes);
	tree2.setObject(vertices, uvs, normals, numberOfTextures, startVertices, texes);

	glm::vec3 cords[25];
	int n = 0;
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 5; j++) {
			float x = (float)(rand() % 20 - 50 + i * 20);
			float z = (float)(rand() % 20 - 50 + j * 20);
			if (x < 10 && z <10 && z >-10 && x > -10) {
				x = 25.0f;
				z = 25.0f;
			}
			cords[n] = glm::vec3(x, 0.0f, z);
			n += 1;
		}
	}

	for (int i = 0; i < 25; i++) {

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
}