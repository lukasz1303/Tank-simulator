#version 330


out vec4 pixelColor;

in vec2 TexCoord;

uniform sampler2D ourTexture;
uniform sampler2D ourTexture2;

void main(void) {
    vec4 kd = mix(texture(ourTexture, TexCoord), texture(ourTexture2, TexCoord), 0.3);
	if(kd.a<0.5){
		discard;
	}


	pixelColor= vec4(kd.rgb, kd.a);

}
