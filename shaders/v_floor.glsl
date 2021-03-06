#version 330

//Zmienne jednorodne
uniform mat4 P;
uniform mat4 V;
uniform mat4 M;

//Atrybuty
in vec4 vertex; //wspolrzedne wierzcholka w przestrzeni modelu
in vec4 normal;
out vec4 l;
out vec4 v;

out vec4 l2;

uniform vec4 lp;
uniform vec4 lp2;

in vec2 aTexCoord;
in vec4 c1;
in vec4 c2;
in vec4 c3;

out vec2 TexCoord;

void main(void) {

    mat4 invTBN = mat4(c1,c2,c3, vec4(0,0,0,1)); 

    l = normalize(invTBN*inverse(M) * lp - invTBN*vertex);
    v = normalize(invTBN*inverse(V*M)*vec4(0, 0, 0, 1) - invTBN * vertex);

    l2 = normalize(invTBN*inverse(M) * lp2 - invTBN*vertex);
    gl_Position=P*V*M*vertex;
    TexCoord = aTexCoord;
}