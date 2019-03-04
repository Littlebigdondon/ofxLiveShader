float vmax(vec3 p)
{
    return max(p.x, min(p.y, p.z));
}
float box(vec3 p, vec3 s)
{
    return vmax(abs(p) - s);
}
vec3 rep(vec3 p, vec3 s)
{
    return mod(p, s) - s * .5;
}

float w(vec3 p)
{
    float d = length(p) - 1.;
    d = min(d, -box(p, vec3(5.)));
    d = min(d, max(-box(p, vec3(4.5)), box(rep(p, vec3(2.)), vec3(.5))));
    return d;
}

vec3 E = vec3(0., .001, 1.);

vec3 nn(vec3 p)
{
    return normalize(vec3(w(p + E.yxx), w(p + E.xyx), w(p + E.xxy)) - w(p));
}

float march(vec3 o, vec3 d, float l, float ML)
{
    for (int i = 0; i < 99; ++i)
    {
        float dd = w(o + d * l);
        l += dd;
        if (dd < .0001 * l || l >= ML) break;
    }
    return l;
}


vec3 O, D, P, N;

float mkd;
float mshine;

uniform float iTime;
uniform vec2 iResolution;

float dirlight(vec3 ld)
{
    return mix(
               max(0., dot(N, ld)) / 3.,
               max(0., pow(dot(N, normalize(ld - D)), mshine)) * (mshine + 8.) / 24.,
               1. - mkd
               );
}
#define t iTime
void main()
{
    vec2 uv = (gl_FragCoord.xy - .5*iResolution.xy) / iResolution.y;
    vec3 col = vec3(0.);
    vec3 at = vec3(0.);

    O = 4. * vec3(sin(t), sin(t * 0.9), cos(t));

    vec3 y = normalize(vec3(0., 1., 0.));
    D = normalize(O - at);
    vec3 x = normalize(cross(y, D));
    D = mat3(x, cross(D, x), D) * normalize(vec3(uv, -1.));

    vec3 kd = vec3(1.);
    for (int i = 0; i < 2; ++i)
    {
        float l = march(O, D, 0.1, 20.);
        P = O + D * l;
        N = nn(P);


        mkd = .5;
        mshine = 100.;

        col += kd * vec3(.8) * dirlight(normalize(vec3(1.)));
        col += kd * vec3(.8) * dirlight(normalize(vec3(1.)));
        O = P;
        D = reflect(D, N);
        kd *= .5;
    }

    gl_FragColor = vec4(sqrt(col),1.0);
}
