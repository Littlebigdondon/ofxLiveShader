uniform vec2 iResolution;
uniform float iTime;

float sphere(vec3 p) {
    return length(p) - 1.;
}
void main() {
    vec2 uv = (gl_FragCoord.xy - .5 * iResolution.xy) / iResolution.y;
    
    gl_FragColor = vec4(sin(iTime * 4.),
                        cos(iTime * 2.),
                        sin(iTime),
                        sphere(vec3(uv * 4., 1.)));
}
