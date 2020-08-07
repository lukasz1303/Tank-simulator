#include "include/Floor.h"
#include <iostream>

Floor::Floor()
{
	createHeightMapArray();
}

void Floor::draw_floor(glm::mat4 P, glm::mat4 V, GLuint tex, GLuint tex1, GLuint tex2, ShaderProgram* sp, glm::vec3 cameraPos){

	sp->use();
	glUniformMatrix4fv(sp->u("P"), 1, false, glm::value_ptr(P));
	glUniformMatrix4fv(sp->u("V"), 1, false, glm::value_ptr(V));

	glm::mat4 M_floor = glm::mat4(1.0f);
	M_floor = glm::translate(M_floor, glm::vec3(0.0f, -0.0f, 0.0f));
	//M_floor = glm::rotate(M_floor, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	//M_floor = glm::scale(M_floor, glm::vec3(50.0f, 2.0f, 50.0f));

	glUniformMatrix4fv(sp->u("M"), 1, false, glm::value_ptr(M_floor));
	glUniformMatrix4fv(sp->u("V"), 1, false, glm::value_ptr(V));
	glUniformMatrix4fv(sp->u("P"), 1, false, glm::value_ptr(P));
	glUniform3f(sp->u("cameraPos"), cameraPos.x, cameraPos.y, cameraPos.z);
	/*glUniform3f(sp->u("cameraPos"), cameraPos.x, cameraPos.y, cameraPos.z);*/

	//glEnableVertexAttribArray(sp->a("c1"));  //Włącz przesyłanie danych do atrybutu normal
	//glVertexAttribPointer(sp->a("c1"), 4, GL_FLOAT, false, 0, myCubeC1); //Wskaż tablicę z danymi dla atrybutu normal
	//glEnableVertexAttribArray(sp->a("c2"));  //Włącz przesyłanie danych do atrybutu normal
	//glVertexAttribPointer(sp->a("c2"), 4, GL_FLOAT, false, 0, myCubeC2); //Wskaż tablicę z danymi dla atrybutu normal
	//glEnableVertexAttribArray(sp->a("c3"));  //Włącz przesyłanie danych do atrybutu normal
	//glVertexAttribPointer(sp->a("c3"), 4, GL_FLOAT, false, 0, myCubeC3); //Wskaż tablicę z danymi dla atrybutu normal

	glUniform4f(sp->u("lp"), -4, 3+calculateHeight(-4,-4), -4, 1);
	glUniform4f(sp->u("lp2"), -1050, 1500, -700, 1);

	glEnableVertexAttribArray(sp->a("vertex"));
	glVertexAttribPointer(sp->a("vertex"), 4, GL_FLOAT, false, 0, verts);

	glEnableVertexAttribArray(sp->a("normal"));
	glVertexAttribPointer(sp->a("normal"), 4, GL_FLOAT, false, 0, normals);

	glEnableVertexAttribArray(sp->a("aTexCoord"));
	glVertexAttribPointer(sp->a("aTexCoord"), 2, GL_FLOAT, false, 0, texCoords);

	glUniform1i(sp->u("ourTexture"), 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, tex);

	/*glUniform1i(sp->u("ourTexture1"), 1);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, tex1);

	glUniform1i(sp->u("ourTexture2"), 2);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, tex2);*/
	


	glDrawArrays(GL_TRIANGLES, 0,240000);

	glDisableVertexAttribArray(sp->a("vertex"));
	glDisableVertexAttribArray(sp->a("normal"));
	glDisableVertexAttribArray(sp->a("aTexCoord"));

}

void Floor::createHeightMapArray()
{
	for (int i = 0; i < heightsMapSize; i++)
		heightMap[i] = new float[heightsMapSize];
}

float Floor::calculateHeight(float X, float Z)
{
	if (X < -1000.0f || X >1000.0f || Z < -1000.0f || Z > 1000.0f) {
		return 0;
	}
	float fx = floor((X + 5 * 200) / 10);
	float fz = floor((Z + 5 * 200) / 10);
	float xh = X - (fx * 10 - 1000);
	float zh = Z - (fz * 10 - 1000);

	float xh1 = 10 - xh;
	float zh1 = 10 - zh;
	float dbl = pow(14.14f - sqrt(xh * xh + zh * zh), 4);
	float dtr = pow(14.14f - sqrt(xh1 * xh1 + zh1 * zh1), 4);



	glm::vec3 p1 = glm::vec3(0.0f);
	glm::vec3 p2 = glm::vec3(0.0f);
	glm::vec3 p3 = glm::vec3(0.0f);

	if (dbl > dtr) {
		p1 = glm::vec3(fx * 10 - 1000, heightMap[(int)fx][(int)fz], fz * 10 - 1000);
		p2 = glm::vec3((fx + 1) * 10 - 1000, heightMap[(int)fx + 1][(int)fz], fz * 10 - 1000);
		p3 = glm::vec3(fx * 10 - 1000, heightMap[(int)fx][(int)fz + 1], (fz + 1) * 10 - 1000);
	}
	else {
		p1 = glm::vec3((fx + 1) * 10 - 1000, heightMap[(int)fx + 1][(int)fz + 1], (fz + 1) * 10 - 1000);
		p2 = glm::vec3((fx + 1) * 10 - 1000, heightMap[(int)fx + 1][(int)fz], fz * 10 - 1000);
		p3 = glm::vec3(fx * 10 - 1000, heightMap[(int)fx][(int)fz + 1], (fz + 1) * 10 - 1000);

	}

	glm::vec2 pos = glm::vec2(X,Z);
	float det = (p2.z - p3.z) * (p1.x - p3.x) + (p3.x - p2.x) * (p1.z - p3.z);
	float l1 = ((p2.z - p3.z) * (pos.x - p3.x) + (p3.x - p2.x) * (pos.y - p3.z)) / det;
	float l2 = ((p3.z - p1.z) * (pos.x - p3.x) + (p1.x - p3.x) * (pos.y - p3.z)) / det;
	float l3 = 1.0f - l1 - l2;
	return (l1 * p1.y + l2 * p2.y + l3 * p3.y);
}
