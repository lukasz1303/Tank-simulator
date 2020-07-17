#include "include/ParticleSystem.h"

float ParticleSystem::rand_gen() {
	return ((float)(rand()) + 1.0f) / ((float)(RAND_MAX)+1.);
}

float ParticleSystem::normalRandom() {
	float v1 = rand_gen();
	float v2 = rand_gen();
	return cos(2 * 3.14f * v2) * sqrt(-2. * log(v1));
}

void ParticleSystem::createParticle(Particle& p) 
{
	p.position = glm::vec3(0.0f, 0.0f, 0.0f);
	p.speed = glm::vec3(normalRandom() * 0.09f +0.1f , normalRandom() * 0.09f + 0, normalRandom() * 0.09+0.1f);
	p.color = glm::vec4(glm::clamp((normalRandom() * 0.5f + 1), 0.2f, 1.0f), glm::clamp((normalRandom() * 0.4f + 0.3f), 0.0f, 0.5f), 0.0f, 1.0f);
	p.dst = 0;
	p.ttl = glm::clamp(normalRandom() * 4 + 9, 0.0f, 10.0f);
}

void ParticleSystem::initializeSystem( int n) 
{
	for (int i = 0; i < n; i++)
		createParticle(system[i]);
	count = n;
	first_frame = true;
}

void ParticleSystem::processSystem(int n, float timestep) 
{
	if (count > 0) {
		count = 0;
		for (int i = 0; i < n; i++)
		{
			system[i].position += system[i].speed * timestep;
			system[i].color.r = glm::clamp(system[i].color.r, 0.5f - system[i].ttl / 30.0f, 0.5f + system[i].ttl / 30.0f);
			system[i].color.g = glm::clamp(system[i].color.g, 0.5f - system[i].ttl / 30.0f, 0.5f + system[i].ttl / 30.0f);
			system[i].color.b = glm::clamp(system[i].color.b, 0.5f - system[i].ttl / 30.0f, 0.5f + system[i].ttl / 30.0f);
			system[i].speed *= gravity;

			system[i].ttl -= timestep;
			if (system[i].ttl > 0) {
				v[i * 4] = system[i].position.x;
				v[i * 4 + 1] = system[i].position.y;
				v[i * 4 + 2] = system[i].position.z;
				v[i * 4 + 3] = 1;

				colors_particles[i * 4] = system[i].color.r;
				colors_particles[i * 4 + 1] = system[i].color.g;
				colors_particles[i * 4 + 2] = system[i].color.b;
				colors_particles[i * 4 + 3] = 1.0f;
				count += 1;
			}
		}
	}

}

void ParticleSystem::drawParticles(glm::mat4 P, glm::mat4 V, ShaderProgram* sp, glm::mat4 M_lufa, glm::vec3 cameraPos,float pitch, float angle, GLuint tex1, GLuint tex2, GLuint tex3, GLuint tex4, GLuint tex5, GLuint tex6, GLuint tex7, GLuint tex8)
{
	processSystem(2000, 0.2f);
	sp->use();
	if (first_frame) {
		M_lufa_copy = M_lufa;
		first_frame = false;
	}

	int numberOfParticles = 200;

	for (int i = 0; i <= numberOfParticles; i++) {
		
		glm::mat4 M_Particle = M_lufa_copy;
		

		M_Particle = glm::translate(M_Particle, system[i].position);
		M_Particle = glm::translate(M_Particle, glm::vec3(0.0f, -0.02f, 2.5f));
		glm::vec3 p = M_Particle * glm::vec4(glm::vec3(0.0f), 1.0f);
		M_Particle = glm::rotate(M_Particle, glm::radians(90.0f - angle), glm::vec3(1.0f, 0.0f, 0.0f));
		M_Particle = glm::rotate(M_Particle, glm::radians(pitch), glm::vec3(1.0f, 0.0f, 0.0f));
		system[i].M = M_Particle;

		system[i].dst = sqrt((p.x - cameraPos.x) * (p.x - cameraPos.x) + (p.y - cameraPos.y) * (p.y - cameraPos.y) + (p.z - cameraPos.z) * (p.z - cameraPos.z));

	}

	quicksort(system, 0, numberOfParticles);

	glUniformMatrix4fv(sp->u("V"), 1, false, glm::value_ptr(V));
	glUniformMatrix4fv(sp->u("P"), 1, false, glm::value_ptr(P));

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	for (int i = numberOfParticles; i >=0; i--) {
		if (system[i].ttl <= 0) {
			continue;
		}
		
		glUniformMatrix4fv(sp->u("M"), 1, false, glm::value_ptr(system[i].M));
		glEnableVertexAttribArray(sp->a("vertex"));
		glVertexAttribPointer(sp->a("vertex"), 4, GL_FLOAT, false, 0, verts);

		glEnableVertexAttribArray(sp->a("aTexCoord"));
		glVertexAttribPointer(sp->a("aTexCoord"), 2, GL_FLOAT, false, 0, texCoords);

		glActiveTexture(GL_TEXTURE0);
		if (system[i].ttl > 9.0f) {
			glBindTexture(GL_TEXTURE_2D, tex1);
		}
		else if (system[i].ttl > 8.0f) {
			glBindTexture(GL_TEXTURE_2D, tex2);
		}
		else if (system[i].ttl > 7.0f) {
			glBindTexture(GL_TEXTURE_2D, tex3);
		}
		else if (system[i].ttl > 6.0f) {
			glBindTexture(GL_TEXTURE_2D, tex4);
		}
		else if (system[i].ttl > 4.75f) {
			glBindTexture(GL_TEXTURE_2D, tex5);
		}
		else if (system[i].ttl > 3.5f) {
			glBindTexture(GL_TEXTURE_2D, tex6);
		}
		else if (system[i].ttl > 2.25f) {
			glBindTexture(GL_TEXTURE_2D, tex7);
		}
		else {
			glBindTexture(GL_TEXTURE_2D, tex8);
		}

		glUniform1i(sp->u("ourTexture1"), 0);

		glDrawArrays(GL_TRIANGLES, 0, 6);
	}

	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//glDrawArrays(GL_POINTS, 0, count);
}

int ParticleSystem::partition(Particle arr[], int low, int high) // dzielimy tablice na dwie czesci, w pierwszej wszystkie liczby sa mniejsze badz rowne x, w drugiej wieksze lub rowne od x
{
	float pivot = arr[low].dst;
	int i = low;
	int j = high;
	Particle tmp;
	while (true)
	{
		while (arr[j].dst > pivot)
			j--;
		while (arr[i].dst < pivot)
			i++;
		if (i < j)
		{
			tmp = arr[i];
			arr[i] = arr[j];
			arr[j] = tmp;
			i++;
			j--;
		}
		else
		{
			return j;
		}

	}
}

void ParticleSystem::quicksort(Particle arr[], int low, int high) {

	if (low < high) {
		int i = partition(arr, low, high);
		quicksort(arr, low, i);
		quicksort(arr, i + 1, high);
	}
}
