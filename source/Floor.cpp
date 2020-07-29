#include "include/Floor.h"
#include <iostream>

Floor::Floor()
{
}

void Floor::draw_floor(glm::mat4 P, glm::mat4 V, GLuint tex, GLuint tex1, GLuint tex2, ShaderProgram* sp){

	sp->use();
	glUniformMatrix4fv(sp->u("P"), 1, false, glm::value_ptr(P));
	glUniformMatrix4fv(sp->u("V"), 1, false, glm::value_ptr(V));

	glm::mat4 M_floor = glm::mat4(1.0f);
	M_floor = glm::translate(M_floor, glm::vec3(0.0f, -0.0f, 0.0f));
	M_floor = glm::rotate(M_floor, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	//M_floor = glm::scale(M_floor, glm::vec3(200.0f, 200.0f, 200.0f));

	glUniformMatrix4fv(sp->u("M"), 1, false, glm::value_ptr(M_floor));
	glUniformMatrix4fv(sp->u("V"), 1, false, glm::value_ptr(V));
	glUniformMatrix4fv(sp->u("P"), 1, false, glm::value_ptr(P));

	glEnableVertexAttribArray(sp->a("c1"));  //Włącz przesyłanie danych do atrybutu normal
	glVertexAttribPointer(sp->a("c1"), 4, GL_FLOAT, false, 0, myCubeC1); //Wskaż tablicę z danymi dla atrybutu normal
	glEnableVertexAttribArray(sp->a("c2"));  //Włącz przesyłanie danych do atrybutu normal
	glVertexAttribPointer(sp->a("c2"), 4, GL_FLOAT, false, 0, myCubeC2); //Wskaż tablicę z danymi dla atrybutu normal
	glEnableVertexAttribArray(sp->a("c3"));  //Włącz przesyłanie danych do atrybutu normal
	glVertexAttribPointer(sp->a("c3"), 4, GL_FLOAT, false, 0, myCubeC3); //Wskaż tablicę z danymi dla atrybutu normal

	glUniform4f(sp->u("lp"), -4, 3, -4, 1);
	glUniform4f(sp->u("lp2"), -50, 20, -50, 1);

	glEnableVertexAttribArray(sp->a("vertex"));
	glVertexAttribPointer(sp->a("vertex"), 4, GL_FLOAT, false, 0, verts);

	glEnableVertexAttribArray(sp->a("normal"));
	glVertexAttribPointer(sp->a("normal"), 4, GL_FLOAT, false, 0, normals);

	glEnableVertexAttribArray(sp->a("aTexCoord"));
	glVertexAttribPointer(sp->a("aTexCoord"), 2, GL_FLOAT, false, 0, texCoords);

	glUniform1i(sp->u("ourTexture"), 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, tex);

	glUniform1i(sp->u("ourTexture1"), 1);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, tex1);

	glUniform1i(sp->u("ourTexture2"), 2);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, tex2);
	


	glDrawArrays(GL_TRIANGLES, 0, vertexCount);

	glDisableVertexAttribArray(sp->a("vertex"));
	glDisableVertexAttribArray(sp->a("normal"));
	glDisableVertexAttribArray(sp->a("aTexCoord"));

}