#version 330

//Zmienne jednorodne
uniform mat4 P;
uniform mat4 V;
uniform mat4 M;

in vec4 vertex;
in vec2 aTexCoord;
in vec3 offset;

uniform vec3 cameraPos;
out float dis;

out vec2 TexCoord;

void main(void) {
    dis = distance(M * (vertex+vec4(offset,0)), vec4(cameraPos,1));
    dis = exp(-pow(dis * 0.009f, 4));
    gl_Position=P*V*M*(vertex+vec4(offset,0));
    TexCoord = aTexCoord;
}
