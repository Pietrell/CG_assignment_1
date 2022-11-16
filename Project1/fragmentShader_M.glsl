#version 330 core

in vec4 ourColor;
 

uniform vec2 resolution;
uniform vec2 mouse;
precision highp float;


const float PI = 3.14159;

const int STAR_COUNT = 200;
const vec3 STAR_COLOR = vec3(1.0);

vec3 ndc = vec3 ((gl_FragCoord.x / resolution.x- 0.5) * 2, (gl_FragCoord.y / resolution.y - 0.5) * 2, gl_FragCoord.z);	
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
            
      gl_FragColor = ourColor;

        
    
}
  


