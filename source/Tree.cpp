#include "include/Tree.h"

void Tree::setObject(std::vector < glm::vec4 > out_vertices, std::vector < glm::vec2 > out_uvs, std::vector < glm::vec4 > out_normals)
{
	vertices = out_vertices;
	uvs = out_uvs;
	normals = out_normals;
}

void Tree::setCords(glm::vec3 coords)
{
	coordinates = coords;
}

void Tree::draw(glm::mat4 P, glm::mat4 V, ShaderProgram* sp,  GLuint tex, GLuint tex2)
{
	glm::mat4 M_tree = glm::mat4(1.0f);
	float color[] = { 1,1,0,1 };
	glVertexAttribPointer(sp->a("color"), 4, GL_FLOAT, false, 0, color);


	M_tree = glm::translate(M_tree, glm::vec3(coordinates));
	M_tree = glm::scale(M_tree, glm::vec3(scale, scale, scale));

	sp->use();
	glUniform4f(sp->u("lp"), -4, 3, -4, 1);
	glUniform4f(sp->u("lp2"), -50, 20, -50, 1);

	glUniformMatrix4fv(sp->u("M"), 1, false, glm::value_ptr(M_tree));
	glUniformMatrix4fv(sp->u("V"), 1, false, glm::value_ptr(V));
	glUniformMatrix4fv(sp->u("P"), 1, false, glm::value_ptr(P));



	glm::mat4 PVM = P * V * M_tree;
	glm::mat4 VM = V * M_tree;
	glm::vec4 Vlp = V * glm::vec4(-4, 3, -4, 1);
	glm::vec4 Vlp2 = V * glm::vec4(-50, 20, -50, 1);
	glUniformMatrix4fv(sp->u("PVM"), 1, false, glm::value_ptr(PVM));
	glUniformMatrix4fv(sp->u("VM"), 1, false, glm::value_ptr(VM));
	glUniform4fv(sp->u("Vlp"), sizeof(Vlp),glm::value_ptr(Vlp));
	glUniform4fv(sp->u("Vlp2"), sizeof(Vlp2), glm::value_ptr(Vlp2));


	glEnableVertexAttribArray(sp->a("vertex"));
	glEnableVertexAttribArray(sp->a("normal"));
	glVertexAttribPointer(sp->a("vertex"), 4, GL_FLOAT, false, 0, &vertices[0]);
	glVertexAttribPointer(sp->a("normal"), 4, GL_FLOAT, false, 0, &normals[0]);

	glEnableVertexAttribArray(sp->a("aTexCoord"));
	glVertexAttribPointer(sp->a("aTexCoord"), 2, GL_FLOAT, false, 0, &uvs[0]);

	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glBindTexture(GL_TEXTURE_2D, tex);
	glUniform1i(sp->u("ourTexture1"), 0);

	glDrawArrays(GL_TRIANGLES, 0, 36444);

	glBindTexture(GL_TEXTURE_2D, tex2);
	glUniform1i(sp->u("ourTexture1"), 0);

	glDrawArrays(GL_TRIANGLES, 36444, vertices.size()-36440);

	glDisableVertexAttribArray(sp->a("vertex"));
	glDisableVertexAttribArray(sp->a("normal"));
	glDisableVertexAttribArray(sp->a("aTexCoord"));
}

void Tree::setScale(float scale)
{
	this->scale = scale;
}
