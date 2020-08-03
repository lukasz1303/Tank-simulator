#version 330


out vec4 pixelColor;
in vec4 n;
in vec4 v;
in vec4 l;

in float d;
in float d2;

in vec4 l2;

in vec2 TexCoord;

uniform sampler2D ourTexture;
uniform float dis;
void main(void) {

    vec4 kd = texture(ourTexture, TexCoord);
    if (kd.a < 0.4) //for example, change to any value suitable
        discard;
    vec4 mn = normalize(n);
    vec4 ml = normalize(l);

    float nl = clamp(dot(mn, ml), 0, 1);

    vec4 ml2 = normalize(l2);

    float nl2 = clamp(dot(mn, ml2), 0, 1);


    vec4 ks = kd;
    float nld = nl2 * d2 + nl * d;
    if (nld < 0.4) {
        nld = 0.4;
    }
    if (nld > 0.9) {
        nld = 0.9;
    }
    pixelColor = mix(vec4(kd.rgb * nld, kd.a),vec4(0.8,0.8,0.8,1.0), dis);
}
