#version 330


out vec4 pixelColor;

in vec2 TexCoord;

uniform sampler2D ourTexture;

void main(void) {
    vec4 kd = texture(ourTexture, TexCoord);
	 if (kd.a < 0.2) //for example, change to any value suitable
        discard;

	pixelColor= vec4(kd.rgb, kd.a);

}
