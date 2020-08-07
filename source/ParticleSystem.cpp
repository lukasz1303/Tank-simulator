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
	p.speed = glm::vec3(glm::clamp(normalRandom() * 0.18f + 0.15f, -0.42f, 0.58f), glm::clamp(normalRandom() * 0.18f + 0.0f, -0.45f, 0.45f), glm::clamp(normalRandom() * 0.18f + 0.05f, -0.45f, 0.5f));

	p.color = glm::vec4(glm::clamp((normalRandom() * 0.5f + 1), 0.2f, 1.0f), glm::clamp((normalRandom() * 0.4f + 0.3f), 0.0f, 0.5f), 0.0f, 1.0f);
	p.dst = 0;
	p.ttl = glm::clamp(normalRandom() * 4 + 9, 0.0f, 12.0f);
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
			
			system[i].ttl -= timestep*2.0f;

			if (system[i].ttl > 0) {

				count += 1;
			}
		}
	}

}

void ParticleSystem::drawParticles(glm::mat4 P, glm::mat4 V, ShaderProgram* sp, glm::mat4 M_lufa, glm::vec3 cameraPos,float pitch, float angle, glm::vec3 speed_vector, GLuint tex)
{
	processSystem(70, 0.06f);
	sp->use();

	int numberOfParticles = 69; //n-1
	if (first_frame) {
		M_lufa_copy = M_lufa;
		pitch_copy = pitch;
		first_frame = false;

		for (int i = 0; i <= numberOfParticles; i++) {
			system[i].speed.y += 8.0f * speed_vector.x * cos(angle * 3.141f / 180.0f);		//y==x
			system[i].speed.z += 8.0f * speed_vector.z * sin(angle * 3.141f / 180.0f);		//z==z
		}
	}

	for (int i = 0; i <= numberOfParticles; i++) {
		
		glm::mat4 M_Particle = M_lufa_copy;
		M_Particle = glm::translate(M_Particle, glm::vec3(0.0f, -0.02f, 2.7f));
		M_Particle = glm::rotate(M_Particle, glm::radians(-pitch_copy), glm::vec3(1.0f, 0.0f, 0.0f));
		M_Particle = glm::translate(M_Particle, system[i].position);

		M_Particle = glm::scale(M_Particle, glm::vec3(0.8f, 0.8f, 0.8f));
		glm::vec4 p = M_Particle * glm::vec4(glm::vec3(0.0f), 1.0f);

		float delta_x = p.x - cameraPos.x;
		float delta_z = p.z - cameraPos.z;
		float theta_radians = -atan2(delta_z, delta_x);

		M_Particle = glm::rotate(M_Particle, glm::radians(theta_radians * 180.0f / 3.141f - 270.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		system[i].M = M_Particle;
		p = M_Particle * glm::vec4(glm::vec3(0.0f), 1.0f);
		system[i].dst = (cameraPos.x - p.x) * (cameraPos.x - p.x) + (cameraPos.x - p.x) * (cameraPos.x - p.x) + (cameraPos.z - p.z) * (cameraPos.z - p.z);	
	}

	quicksort(system, 0, numberOfParticles);

	glUniformMatrix4fv(sp->u("V"), 1, false, glm::value_ptr(V));
	glUniformMatrix4fv(sp->u("P"), 1, false, glm::value_ptr(P));

	glEnableVertexAttribArray(sp->a("vertex"));
	glEnableVertexAttribArray(sp->a("aTexCoord"));
	glEnableVertexAttribArray(sp->a("color"));

	float coordX;
	float coordY;

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	for (int i = numberOfParticles; i >=0; i--) {
		if (system[i].ttl <= 0) {
			continue;
		}
		
		glUniformMatrix4fv(sp->u("M"), 1, false, glm::value_ptr(system[i].M));
		glVertexAttribPointer(sp->a("vertex"), 4, GL_FLOAT, false, 0, verts);

		coordX = 1 - (round(system[i].ttl * 7) / 12 - floor(round(system[i].ttl * 7) / 12)) - 1.0f / 12.0f;
		coordY = 1-floor(round(system[i].ttl * 7) / 12) / 7 - 1.0f / 12.0f - 1.0f/14.0f;

		float texCoords[12] = {
		  coordX + 1.0f / 12.0f,	coordY,						//A
		  coordX,					coordY + 1.0f / 7.0f,		//B
		  coordX,					coordY,						//C

		  coordX + 1.0f / 12.0f,	coordY,						//A
		  coordX + 1.0f / 12.0f,	coordY + 1.0f / 7.0f,		//D
		  coordX,					coordY + 1.0f / 7.0f,		//B
		};

		glVertexAttribPointer(sp->a("aTexCoord"), 2, GL_FLOAT, false, 0, texCoords);
		glActiveTexture(GL_TEXTURE0);
		
		glBindTexture(GL_TEXTURE_2D, tex);
		glUniform1i(sp->u("ourTexture1"), 0);

		glDrawArrays(GL_TRIANGLES, 0, 6);
	}
	glDisable(GL_BLEND);
	glDisableVertexAttribArray(sp->a("vertex"));
	glDisableVertexAttribArray(sp->a("aTexCoord"));
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
