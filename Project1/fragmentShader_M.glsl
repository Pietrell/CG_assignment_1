#version 330 core

in vec4 ourColor;
out vec4 FragColor;  


precision highp float;


const float PI = 3.14159;

const int STAR_COUNT = 200;
const vec3 STAR_COLOR = vec3(1.0);



float rand(vec2 co){
    return fract(sin(dot(co, vec2(12.9898, 78.233))) * 43758.5453);
}

void point(vec2 pos, float size) {
    pos *= vec2(1280,720);
    float d = distance(gl_FragCoord.xy, pos);
    gl_FragColor.rgb += smoothstep(size, 0.0, d) * STAR_COLOR;
}

void stars() {
    for (int i = 1; i < STAR_COUNT; i++) {
        float sx = rand(vec2(i, -i));
        float sy = rand(vec2(-sx, sx));
        float size = rand(vec2(sx, sy)) ;
     
    }
}

void main() {
    FragColor = ourColor;
         
    stars();
    
}
  


