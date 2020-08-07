#version 330


out vec4 pixelColor;

in vec2 TexCoord;

uniform sampler2D ourTexture;
in float dis;
void main(void) {
    vec4 kd = texture(ourTexture, TexCoord);
	if(kd.a<0.5){
		discard;
	}
	float factor = (TexCoord.y )/3.3;
	factor = clamp(factor,0,1);
	pixelColor= mix(vec4(kd.rgb, kd.a),vec4(0.8,0.8,0.8,1.0),0);

}
