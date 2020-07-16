#include "Sky.h"

void Sky::draw_sky(glm::mat4 P, glm::mat4 V, Texture skybox[], ShaderProgram* sp, glm::vec3 speed_vector)
{
	sp->use();

	glm::mat4 M_skyBox = glm::mat4(1.0f);

	M_skyBox = glm::translate(M_skyBox, speed_vector);
	M_skyBox = glm::rotate(M_skyBox, glm::radians(270.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	M_skyBox = glm::scale(M_skyBox, glm::vec3(65.0f, 65.0f, 65.0f));

	glUniformMatrix4fv(sp->u("M"), 1, false, glm::value_ptr(M_skyBox));
	glUniformMatrix4fv(sp->u("V"), 1, false, glm::value_ptr(V));
	glUniformMatrix4fv(sp->u("P"), 1, false, glm::value_ptr(P));
	glEnableVertexAttribArray(sp->a("vertex"));
	glVertexAttribPointer(sp->a("vertex"), 4, GL_FLOAT, false, 0, vertices);


	glEnableVertexAttribArray(sp->a("aTexCoord"));
	glVertexAttribPointer(sp->a("aTexCoord"), 2, GL_FLOAT, false, 0, texCoords);


	for (int i = 0; i < 36; i += 6) {
		glBindTexture(GL_TEXTURE_2D, skybox[i / 6].tex);
		glUniform1i(sp->u("ourTexture1"), 0);
		glDrawArrays(GL_TRIANGLES, i, 6);
	}

	glDisableVertexAttribArray(sp->a("vertex"));
	glDisableVertexAttribArray(sp->a("aTexCoord"));
}

Sky::Sky()
{
}

