#pragma once
#include <include\shaderprogram.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>


class ParticleSystem
{

private:
	float verts[24] = {
	  1.0f,-1.0f,0.0f,1.0f, //A
	 -1.0f, 1.0f,0.0f,1.0f, //B
	 -1.0f,-1.0f,0.0f,1.0f, //C

	  1.0f,-1.0f,0.0f,1.0f, //A
	  1.0f, 1.0f,0.0f,1.0f, //D
	 -1.0f, 1.0f,0.0f,1.0f, //B
	};

	//Tablica wspólrzednych teksturowania
	float texCoords[12] = {
	  1.0f, 0.0f,   //A
	  0.0f, 1.0f,    //B
	  0.0f, 0.0f,    //C

	  1.0f, 0.0f,    //A
	  1.0f, 1.0f,    //D
	  0.0f, 1.0f,    //B
	};


	float v[8000];
	float colors_particles[8000];
	
	glm::mat4 M_lufa_copy = glm::mat4(1.0f);

	bool first_frame = true;
	struct Particle {
		glm::vec3 position; //Po³o¿enie cz¹stki
		glm::vec3 speed; //Prêdkoœæ cz¹stki
		glm::vec4 color;
		float ttl; //Czas ¿ycia
	};
	const int n = 2000; //Liczba cz¹stek

	Particle system[2000]; //Tablica cz¹stek

	glm::vec3 gravity = glm::vec3(0.95f, 0.95f, 0.95f); //Wektor grawitacji

	float rand_gen();
	float normalRandom();

	void createParticle(Particle& p);

public:
	int count = 0;
	void initializeSystem(int n);
	void processSystem(int n, float timestep);
	void drawParticles(glm::mat4 P, glm::mat4 V, ShaderProgram* sp, glm::mat4 M_lufa, GLuint tex);
	
};
