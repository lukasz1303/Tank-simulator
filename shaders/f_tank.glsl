#version 330


out vec4 pixelColor;
in vec4 n;
in vec4 v;
in vec4 l;

in float d;
in float d2;

in vec4 l2;

in vec2 TexCoord;

uniform sampler2D ourTexture1;
uniform sampler2D ourTexture2;

void main(void) {

    vec4 kd = mix(texture(ourTexture1, TexCoord),texture(ourTexture2, TexCoord),0.2);
    if (kd.a < 0.5) //for example, change to any value suitable
        discard;
    vec4 mn = normalize(n);
    vec4 mv = normalize(v);
    vec4 ml = normalize(l);

    vec4 r = reflect(-ml, mn);

    float nl = clamp(dot(mn, ml), 0, 1);
    float rv = pow(clamp(dot(r, mv), 0, 1),50);

    vec4 ml2 = normalize(l2);

    vec4 r2 = reflect(-ml2, mn);

    float nl2 = clamp(dot(mn, ml2), 0, 1);
    float rv2 = pow(clamp(dot(r2, mv), 0, 1), 50);

    vec4 ks = kd;
    float nld = nl2 * d2 + nl * d;
    if (nld < 0.15) {
        nld = 0.15;
    }
    if (nld > 0.9) {
        nld = 0.9;
    }
	pixelColor= vec4(kd.rgb*nld, kd.a) + vec4(ks.rgb*rv, 0)*d + vec4(ks.rgb * rv2, 0)*d2;
}
