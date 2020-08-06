#include "Grass.h"

Grass::Grass()
{
}

void Grass::draw(glm::mat4 P, glm::mat4 V, ShaderProgram* sp, glm::vec4 tankPosition, Frustrum& frustrum, GLuint tex, GLuint tex2)
{
	positionsFrustrum.clear();
	for (int i = 0; i < positions.size(); i++) {
		if (frustrum.pointInFrustum(positions[i])) {
			positionsFrustrum.push_back(positions[i]);
		}
	}
	if (positionsFrustrum.size() == 0) {
		return;
	}
	sp->use();
	glEnableVertexAttribArray(sp->a("vertex"));
	glEnableVertexAttribArray(sp->a("aTexCoord"));
	glEnableVertexAttribArray(sp->a("offset"));
	glUniformMatrix4fv(sp->u("V"), 1, false, glm::value_ptr(V));
	glUniformMatrix4fv(sp->u("P"), 1, false, glm::value_ptr(P));

	glVertexAttribPointer(sp->a("vertex"), 4, GL_FLOAT, false, 0, verts);
	glVertexAttribPointer(sp->a("aTexCoord"), 2, GL_FLOAT, false, 0, texCoords);
	glVertexAttribPointer(sp->a("offset"), 3, GL_FLOAT, GL_FALSE, 0, &positionsFrustrum[0]);
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
	//M_grass = glm::rotate(M_grass, glm::radians(180.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	//M_grass = glm::scale(M_grass, glm::vec3(0.7f, 0.7f, 0.7f));
	glUniformMatrix4fv(sp->u("M"), 1, false, glm::value_ptr(M_grass));
	
	glDrawArraysInstanced(GL_TRIANGLES, 0, 12, positionsFrustrum.size());
	glDisableVertexAttribArray(sp->a("vertex"));
	glDisableVertexAttribArray(sp->a("aTexCoord"));
	glDisableVertexAttribArray(sp->a("offset"));
	glVertexAttribDivisor(2, 0);

}

void Grass::setPositions(Floor& ground)
{
	glm::vec3 position;
	float r, r2;
	position.y = 0.9f;
	for (int i = 0; i < 2000; i++) {
		r2 = rand() % 500;
		r = pow(r2, 1.0f / 1.3f);
		float d = (rand() % 3600)/10.0f;

		position.x = r * sin(d* 3.141f / 180.0f);
		position.z = r * cos(d * 3.141f / 180.0f);
		position.y = ground.calculateHeight(position.x, position.z) + 0.9f;

		this->positions.push_back(position);
	}

}
