#pragma once
#include <include\shaderprogram.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>


class ParticleSystem
{

private:
	float verts[24] = {
	  0.2f,-0.2f,0.0f,1.0f, //A
	 -0.2f, 0.2f,0.0f,1.0f, //B
	 -0.2f,-0.2f,0.0f,1.0f, //C

	  0.2f,-0.2f,0.0f,1.0f, //A
	  0.2f, 0.2f,0.0f,1.0f, //D
	 -0.2f, 0.2f,0.0f,1.0f, //B
	};

	//Tablica wsp�lrzednych teksturowania
	float texCoords[12] = {
	  1.0f, 0.0f,   //A
	  0.0f, 1.0f,    //B
	  0.0f, 0.0f,    //C

	  1.0f, 0.0f,    //A
	  1.0f, 1.0f,    //D
	  0.0f, 1.0f,    //B
	};

	float colors[24] = {
	1.0f,0.0f,0.0f,1.0f,
	1.0f,0.0f,0.0f,1.0f,
	1.0f,0.0f,0.0f,1.0f,

	1.0f,0.0f,0.0f,1.0f,
	1.0f,0.0f,0.0f,1.0f,
	1.0f,0.0f,0.0f,1.0f };


	float v[8000];
	float colors_particles[8000];
	
	glm::mat4 M_lufa_copy = glm::mat4(1.0f);
	float pitch_copy = 0;
	glm::vec3 speed_vector_copy;

	bool first_frame = true;
	struct Particle {
		glm::vec3 position; //Po�o�enie cz�stki
		glm::vec3 speed; //Pr�dko�� cz�stki
		glm::vec4 color;
		float dst;
		float ttl; //Czas �ycia
		glm::mat4 M;
	};
	const int n = 2000; //Liczba cz�stek

	Particle system[2000]; //Tablica cz�stek

	glm::vec3 gravity = glm::vec3(0.92f, 0.92f, 0.92f); //Wektor grawitacji

	float rand_gen();
	float normalRandom();

	void createParticle(Particle& p);

public:
	int count = 0;
	void initializeSystem(int n);
	void processSystem(int n, float timestep);
	void drawParticles(glm::mat4 P, glm::mat4 V, ShaderProgram* sp, glm::mat4 M_lufa, glm::vec3 cameraPos, float pitch,float angle, glm::vec3 speed_vector,GLuint tex);
	int partition(Particle arr[], int low, int high);
	void quicksort(Particle arr[], int low, int high);
};
