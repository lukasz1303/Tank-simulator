#include "include/Tree.h"

void Tree::setObject(std::vector < glm::vec4 > out_vertices, std::vector < glm::vec2 > out_uvs, std::vector < glm::vec4 > out_normals, int numberOfTextures, std::vector<int> startVertices, std::vector<GLuint> texes)
{
	vertices = out_vertices;
	uvs = out_uvs;
	normals = out_normals;
	this->numberOfTextures = numberOfTextures;
	this->startVertices = startVertices;
	this->texes = texes;
}

void Tree::setCords(glm::vec3 coords)
{
	coordinates = coords;
}

void Tree::draw(glm::mat4 P, glm::mat4 V, ShaderProgram* sp,  GLuint tex, GLuint tex2)
{
	sp->use();
	glm::mat4 M_tree = glm::mat4(1.0f);
	float color[] = { 1,1,0,1 };
	glVertexAttribPointer(sp->a("color"), 4, GL_FLOAT, false, 0, color);

	M_tree = glm::translate(M_tree, glm::vec3(coordinates));
	M_tree = glm::scale(M_tree, glm::vec3(scale, scale, scale));


	glUniform4f(sp->u("lp"), -4.0, 3.5, -4.0, 1.0);
	glUniform4f(sp->u("lp2"), -50.0, 20.0, -50.0, 1.0);

	glUniformMatrix4fv(sp->u("M"), 1, false, glm::value_ptr(M_tree));
	glUniformMatrix4fv(sp->u("V"), 1, false, glm::value_ptr(V));
	glUniformMatrix4fv(sp->u("P"), 1, false, glm::value_ptr(P));

	glEnableVertexAttribArray(sp->a("vertex"));
	glEnableVertexAttribArray(sp->a("normal"));
	glEnableVertexAttribArray(sp->a("aTexCoord"));

	glVertexAttribPointer(sp->a("vertex"), 4, GL_FLOAT, false, 0, &vertices[0]);
	glVertexAttribPointer(sp->a("normal"), 4, GL_FLOAT, false, 0, &normals[0]);
	glVertexAttribPointer(sp->a("aTexCoord"), 2, GL_FLOAT, false, 0, &uvs[0]);

	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	for (int i = 0; i < numberOfTextures; i++) {
		glBindTexture(GL_TEXTURE_2D, texes[i]);
		glUniform1i(sp->u("ourTexture1"), 0);
		glDrawArrays(GL_TRIANGLES, startVertices[i], startVertices[i+1]-startVertices[i]);
	}

	glDisableVertexAttribArray(sp->a("vertex"));
	glDisableVertexAttribArray(sp->a("normal"));
	glDisableVertexAttribArray(sp->a("aTexCoord"));
}

void Tree::setScale(float scale)
{
	this->scale = scale;
}
