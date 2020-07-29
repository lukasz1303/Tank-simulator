#version 330

out vec4 pixelColor; //Zmienna wyjsciowa fragment shadera. Zapisuje sie do niej ostateczny (prawie) kolor piksela
in vec4 l;
in vec4 v;
in vec4 l2;

in vec2 TexCoord;

uniform sampler2D ourTexture;
uniform sampler2D ourTexture1;
uniform sampler2D ourTexture2;

vec2 parallaxTexCoords(vec4 v, vec2 t, float h, float s){
    vec2 ti = -h*v.xy/s;
    float hi = -h/s;

    vec2 tc = t;
    float hc = h;
    float ht = h*texture(ourTexture2,tc).r;

    if(v.z<=0) discard;

    while (hc> ht){
        tc = tc +ti;
        if(tc.x < 0 || tc.x >500 || tc.y < 0 || tc.y >500 ) discard;
        hc = hc + hi;
        ht = h*texture(ourTexture2,tc).r;
    }

    vec2 tco = tc - ti;
    float hco = hc - hi;
    float hto = h*texture(ourTexture2,tco).r;

    float x = (hco - hto) / (hco - hto - (hc - ht));

    return (1-x)*tco+x*tc;
}


void main(void) {
   
   
    vec4 mv = normalize(v);
    vec2 nt = parallaxTexCoords(mv,TexCoord,0.1,100);
    
    vec4 ml = normalize(l);
    vec4 mn = normalize(vec4(texture(ourTexture1, nt).rgb*2-1,0));

    vec4 kd = texture(ourTexture, nt);
    float nl1 = clamp(dot(mn, ml)+0.1, 0, 1);

    vec4 ml2 = normalize(l2);
    float nl2 = clamp(dot(mn, ml2)+0.1, 0, 1);

    float nl = nl1 +nl2;
    if(nl1+nl2>0.9)
        nl = 0.9;
    if(nl1+nl2<0.2)
        nl = 0.2;
    pixelColor = vec4(kd.rgb * nl, kd.a);

}
