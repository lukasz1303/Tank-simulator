#include "Grass.h"

Grass::Grass()
{
	setPositions();
}

void Grass::draw(glm::mat4 P, glm::mat4 V, ShaderProgram* sp, GLuint tex, GLuint tex2)
{
	sp->use();
	glEnableVertexAttribArray(sp->a("vertex"));
	glEnableVertexAttribArray(sp->a("aTexCoord"));
	glUniformMatrix4fv(sp->u("V"), 1, false, glm::value_ptr(V));
	glUniformMatrix4fv(sp->u("P"), 1, false, glm::value_ptr(P));

	glVertexAttribPointer(sp->a("vertex"), 4, GL_FLOAT, false, 0, verts);
	glVertexAttribPointer(sp->a("aTexCoord"), 2, GL_FLOAT, false, 0, texCoords);
	
	////glVertexAttribPointer(sp->a("offset"), 4, GL_FLOAT, false, 0, translations);
	//glBindBuffer(GL_ARRAY_BUFFER, 0);
	//glVertexAttribDivisor(4, 1);


	/*unsigned int instanceVBO;
	glGenBuffers(1, &instanceVBO);
	glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * 10000, &positions[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);*/


	/*glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);*/
	glVertexAttribPointer(sp->a("offset"), 3, GL_FLOAT, GL_FALSE, 0, &positions[0]);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glVertexAttribDivisor(2, 1);


	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, tex);
	glUniform1i(sp->u("ourTexture"), 0);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, tex2);
	glUniform1i(sp->u("ourTexture2"), 1);
	glm::mat4 M_grass;
	M_grass = glm::mat4(1.0f);
	M_grass = glm::rotate(M_grass, glm::radians(180.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	M_grass = glm::scale(M_grass, glm::vec3(0.7f, 0.7f, 0.7f));
	glUniformMatrix4fv(sp->u("M"), 1, false, glm::value_ptr(M_grass));
	
	glDrawArraysInstanced(GL_TRIANGLES, 0, 18,10000);
	glDisableVertexAttribArray(sp->a("vertex"));
	glDisableVertexAttribArray(sp->a("aTexCoord"));
	glDisableVertexAttribArray(sp->a("offset"));
	glVertexAttribDivisor(2, 0);

}

void Grass::setPositions()
{
	glm::vec3 position;
	float r, r2;
	position.y = -0.9f;
	for (int i = 0; i < 10000; i++) {
		r2 = rand() % 600 - 300;
		r = pow(r2, 1.0f / 1.3f);
		/*if (r2 < 0) {
			r = -sqrt(-r2);
		}
		else {
			r = sqrt(r2);
		}*/
		float d = (rand() % 360);
		position.x = r * sin(d* 3.141f / 180.0f);
		position.z = r * cos(d * 3.141f / 180.0f);
		this->positions.push_back(position);
	}

}
