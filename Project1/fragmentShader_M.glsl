#version 330 core
#extension GL_OES_standard_derivatives : enable

in vec4 ourColor;
out vec4 FragColor;  
 
uniform int sceltaFS;

uniform vec2 res;


precision highp float;

uniform float time;
uniform vec2 mouse;
uniform vec2 resolution;
const float PI = 3.14159;

const int STAR_COUNT = 400;
const vec3 STAR_COLOR = vec3(1.0);

const vec3 SUN_DIR = vec3(0.940721, 0.28221626, 0.18814417);
const vec3 SUN_COLOR = vec3(0.3, 0.21, 0.165);


float rand(vec2 co){
    return fract(sin(dot(co, vec2(12.9898, 78.233))) * 43758.5453);
}

void point(vec2 pos, float size) {
    pos *= resolution;
    float d = distance(gl_FragCoord.xy, pos);
    gl_FragColor.rgb += smoothstep(size, 0.0, d) * STAR_COLOR;
}

void stars() {
    for (int i = 1; i < STAR_COUNT; i++) {
        float sx = rand(vec2(i, -i));
        float sy = rand(vec2(-sx, sx));
        float size = rand(vec2(sx, sy)) ;
        point(vec2(sx * 2.0 - 1.0 + mouse.x, sy * 2.0 - 1.0 + mouse.y), size * size * 2.0 + 1.0);
    }
}


void main() {
    gl_FragColor = vec4(0.0, 0.0, 0.0, 1.0);
    
    vec2 rotation = vec2((mouse.x * 5.0 - 1.0), (mouse.y - 5.00)) * -PI;
    float xzLen = cos(rotation.y);
    vec3 dir = vec3(xzLen * sin(-rotation.x), sin(rotation.y), xzLen * cos(rotation.x));    
    vec2 res = 1.0 / resolution;    
    stars();
    FragColor = vec4(ourColor);
    
}