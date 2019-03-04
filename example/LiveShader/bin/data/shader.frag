
#version 120

uniform vec2 iResolution;
uniform float iTime;

float XOR(float a, float b) {
    return a*(1.-b) + b*(1.-a);
}

void main()
{
    vec2 uv = (gl_FragCoord.xy - .5 * iResolution.xy) / iResolution.y;

    vec3 col = vec3(0);

    float a = .785;
    float s = sin(a);
    float c = cos(a);
    uv *= mat2(c, -s, s, c);
    uv *= 15.;

    vec2 gv = fract(uv) - .5;
    vec2 id = floor(uv);

    float m = 0.;
    float t = iTime;

    for (float y = -1.; y <= 1.; y++) {
    	for (float x = -1.; x <= 1.; x++) {
            vec2 offs = vec2(x,y);

    		float d = length(gv-offs);
    		float dist = length(id+offs)*.3;

            float r = mix(.3, 1.5, sin(dist-t)*.5+.5);

            m = XOR(m, smoothstep(r, r*.9,d));
        }
    }

    //col.rg = gv;
    col += m;

    gl_FragColor = vec4(col,1.0);
}
