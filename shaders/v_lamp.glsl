#version 330

//Zmienne jednorodne
uniform mat4 P;
uniform mat4 V;
uniform mat4 M;

in vec4 vertex;
in vec2 aTexCoord;
uniform vec3 cameraPos;

out vec2 TexCoord;
out float dis;

void main(void) {

    dis = distance(M * vertex, vec4(cameraPos,1));
    dis = exp(-pow(dis * 0.007f, 1.5));
    gl_Position=P*V*M*vertex;
    TexCoord = aTexCoord;
}
