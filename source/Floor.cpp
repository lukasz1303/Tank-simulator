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
	/*glUniform3f(sp->u("cameraPos"), cameraPos.x, cameraPos.y, cameraPos.z);*/

	//glEnableVertexAttribArray(sp->a("c1"));  //Włącz przesyłanie danych do atrybutu normal
	//glVertexAttribPointer(sp->a("c1"), 4, GL_FLOAT, false, 0, myCubeC1); //Wskaż tablicę z danymi dla atrybutu normal
	//glEnableVertexAttribArray(sp->a("c2"));  //Włącz przesyłanie danych do atrybutu normal
	//glVertexAttribPointer(sp->a("c2"), 4, GL_FLOAT, false, 0, myCubeC2); //Wskaż tablicę z danymi dla atrybutu normal
	//glEnableVertexAttribArray(sp->a("c3"));  //Włącz przesyłanie danych do atrybutu normal
	//glVertexAttribPointer(sp->a("c3"), 4, GL_FLOAT, false, 0, myCubeC3); //Wskaż tablicę z danymi dla atrybutu normal

	glUniform4f(sp->u("lp"), -4, 3, -4, 1);
	glUniform4f(sp->u("lp2"), -1050, 4000, -1050, 1);

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
	


	glDrawArrays(GL_TRIANGLES, 0,15000);

	glDisableVertexAttribArray(sp->a("vertex"));
	glDisableVertexAttribArray(sp->a("normal"));
	glDisableVertexAttribArray(sp->a("aTexCoord"));

}

void Floor::createHeightMapArray()
{
	for (int i = 0; i < heightsMapSize; i++)
		heightMap[i] = new float[heightsMapSize];
}

void Floor::calculateNormals()
{
	float heightL = 0;
	float heightR = 0;
	float heightD = 0;
	float heightU = 0;
	for (int i = 0; i < 51; i++) {
		for (int j = 0; j < 51; j++) {
			if (i == 0) {
				heightR = 0;
			}
			else {
				heightR = heightMap[i - 1][j];
			}
			if (i == 50) {
				heightL = 0;
			}
			else {
				heightL = heightMap[i + 1][j];
			}
			if (j == 0) {
				heightD = 0;
			}
			else {
				heightD = heightMap[i][j - 1];
			}
			if (j == 50) {
				heightU = 0;
			}
			else {
				heightU = heightMap[i][j + 1];
			}
			normals[i] = heightL - heightR;
			normals[i+1] = 1.0f;
			normals[i+2] = heightD - heightU;
			normals[i+3] = 0.0f;
		
		}
	}
}

float Floor::calculateHeight(float X, float Z)
{
	if (X < -1000.0f || X >1000.0f || Z < -1000.0f || Z > 1000.0f) {
		return 0;
	}
	float fx = floor((X + 20 * 50) / 40);
	float fz = floor((Z + 20 * 50) / 40);
	float xh = X - (fx * 40 - 1000);
	float zh = Z - (fz * 40 - 1000);

	float xh1 = 40 - xh;
	float zh1 = 40 - zh;
	float dbl = pow(56.56854f - sqrt(xh * xh + zh * zh), 4);
	float dtr = pow(56.56854f - sqrt(xh1 * xh1 + zh1 * zh1), 4);



	glm::vec3 p1 = glm::vec3(0.0f);
	glm::vec3 p2 = glm::vec3(0.0f);
	glm::vec3 p3 = glm::vec3(0.0f);

	if (dbl > dtr) {
		p1 = glm::vec3(fx * 40 - 1000, heightMap[(int)fx][(int)fz], fz * 40 - 1000);
		p2 = glm::vec3((fx + 1) * 40 - 1000, heightMap[(int)fx + 1][(int)fz], fz * 40 - 1000);
		p3 = glm::vec3(fx * 40 - 1000, heightMap[(int)fx][(int)fz + 1], (fz + 1) * 40 - 1000);
	}
	else {
		p1 = glm::vec3((fx + 1) * 40 - 1000, heightMap[(int)fx + 1][(int)fz + 1], (fz + 1) * 40 - 1000);
		p2 = glm::vec3((fx + 1) * 40 - 1000, heightMap[(int)fx + 1][(int)fz], fz * 40 - 1000);
		p3 = glm::vec3(fx * 40 - 1000, heightMap[(int)fx][(int)fz + 1], (fz + 1) * 40 - 1000);

	}

	glm::vec2 pos = glm::vec2(X,Z);
	float det = (p2.z - p3.z) * (p1.x - p3.x) + (p3.x - p2.x) * (p1.z - p3.z);
	float l1 = ((p2.z - p3.z) * (pos.x - p3.x) + (p3.x - p2.x) * (pos.y - p3.z)) / det;
	float l2 = ((p3.z - p1.z) * (pos.x - p3.x) + (p1.x - p3.x) * (pos.y - p3.z)) / det;
	float l3 = 1.0f - l1 - l2;
	return (l1 * p1.y + l2 * p2.y + l3 * p3.y);
}
