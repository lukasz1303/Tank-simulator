#version 330

//Zmienne jednorodne
uniform mat4 P;
uniform mat4 V;
uniform mat4 M;


//Atrybuty
in vec4 vertex; //wspolrzedne wierzcholka w przestrzeni modelu
in vec4 normal;
out vec4 iC;
out vec4 l;
out vec4 n;
out vec4 v;

out vec4 l2;

out float d;
out float d2;

uniform vec4 lp;
uniform vec4 lp2;

in vec2 aTexCoord;

out vec2 TexCoord;

void main(void) {

    vec4 VMv = V*M * vertex;
    l = normalize(V*lp - VMv);
    n = normalize(V*M * normal);
    v = normalize(vec4(0, 0, 0, 1) - VMv);

    l2 = normalize(V*lp2 - VMv);

    d = distance(VMv, V*lp);
    d = 1 - (d / 100.0f);
    if (d < 0.1)
        d = 0.1;
    if (d > 0.9)
        d = 0.9;

    d2 = distance(VMv, V*lp2);
    d2 = 1 - (d2 / 100.0f);
    if (d2 < 0.1)
        d2 = 0.1;
    if (d2 > 0.9)
        d2 = 0.9;

    gl_Position = P*V*M * vertex;
    TexCoord = aTexCoord;
}
