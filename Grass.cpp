#include "Grass.h"

Grass::Grass()
{
	setPositions();
}

void Grass::draw(glm::mat4 P, glm::mat4 V, ShaderProgram* sp, GLuint tex)
{
	sp->use();
	glEnableVertexAttribArray(sp->a("vertex"));
	glEnableVertexAttribArray(sp->a("aTexCoord"));
	glUniformMatrix4fv(sp->u("V"), 1, false, glm::value_ptr(V));
	glUniformMatrix4fv(sp->u("P"), 1, false, glm::value_ptr(P));

	glVertexAttribPointer(sp->a("vertex"), 4, GL_FLOAT, false, 0, verts);
	glVertexAttribPointer(sp->a("aTexCoord"), 2, GL_FLOAT, false, 0, texCoords);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, tex);
	glUniform1i(sp->u("ourTexture1"), 0);
	glm::mat4 M_grass;
	for (int j = 0; j < 1000; j++) {
		M_grass = glm::mat4(1.0f);
		M_grass = glm::translate(M_grass, positions[j]);
		M_grass = glm::rotate(M_grass, glm::radians(180.0f), glm::vec3(0.0f, 0.0f, 1.0f));

		glUniformMatrix4fv(sp->u("M"), 1, false, glm::value_ptr(M_grass));
		glDrawArrays(GL_TRIANGLES, 0, 18);

	}
	glDisableVertexAttribArray(sp->a("vertex"));
	glDisableVertexAttribArray(sp->a("aTexCoord"));

}

void Grass::setPositions()
{
	glm::vec3 position;
	float r, r2;
	position.y = 0.7f;
	for (int i = 0; i < 1000; i++) {
		r2 = rand() % 10000 - 5000;
		if (r2 < 0) {
			r = -sqrt(-r2);
		}
		else {
			r = sqrt(r2);
		}
		float d = (rand() % 360);
		position.x = r * sin(d* 3.141f / 180.0f);
		position.z = r * cos(d * 3.141f / 180.0f);
		this->positions.push_back(position);
	}

}
