#version 330


out vec4 pixelColor;

in vec2 TexCoord;

uniform sampler2D ourTexture;
uniform sampler2D ourTexture2;
in float dis;

void main(void) {
    vec4 kd = mix(texture(ourTexture, TexCoord), texture(ourTexture2, TexCoord), 0.2);
	if(kd.a<0.5){
		discard;
	}


	pixelColor= mix(vec4(kd.rgb, kd.a),vec4(0.8,0.8,0.8,1.0),1-dis);

}
