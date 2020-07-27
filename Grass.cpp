#include "Grass.h"

void Grass::draw(glm::mat4 P, glm::mat4 V, ShaderProgram* sp, GLuint tex)
{
	sp->use();

	glm::mat4 M_grass = glm::mat4(1.0f);
	M_grass = glm::translate(M_grass, glm::vec3(-5.0f, 0.7f, -5.0f));
	M_grass = glm::rotate(M_grass, glm::radians(180.0f), glm::vec3(0.0f, 0.0f, 1.0f));

	glUniformMatrix4fv(sp->u("M"), 1, false, glm::value_ptr(M_grass));
	glUniformMatrix4fv(sp->u("V"), 1, false, glm::value_ptr(V));
	glUniformMatrix4fv(sp->u("P"), 1, false, glm::value_ptr(P));

	glEnableVertexAttribArray(sp->a("vertex"));
	glEnableVertexAttribArray(sp->a("aTexCoord"));
	glVertexAttribPointer(sp->a("vertex"), 4, GL_FLOAT, false, 0, verts);
	glVertexAttribPointer(sp->a("aTexCoord"), 2, GL_FLOAT, false, 0, texCoords);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, tex);
	glUniform1i(sp->u("ourTexture1"), 0);

	glDrawArrays(GL_TRIANGLES, 0, 6);

	for (int i = 0; i < 2; i++) {
		M_grass = glm::rotate(M_grass, glm::radians(60.0f), glm::vec3(0.0f, 1.0f, 0.0f));

		glUniformMatrix4fv(sp->u("M"), 1, false, glm::value_ptr(M_grass));
		glDrawArrays(GL_TRIANGLES, 0, 6);
	}
	
	glDisableVertexAttribArray(sp->a("vertex"));
	glDisableVertexAttribArray(sp->a("aTexCoord"));

}
