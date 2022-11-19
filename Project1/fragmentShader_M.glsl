#version 330 core
#extension GL_OES_standard_derivatives : enable
#define PI 3.14159

in vec4 ourColor;
 
uniform int sceltaFS;
uniform float time;
uniform vec2 resolution;
uniform vec2 mouse;
precision highp float;




const int STAR_COUNT = 500;


float rand(vec2 co){
    return fract(sin(dot(co, vec2(12.9898, 78.233))) * 4758.5453);
}

void point(vec2 pos, float size) {
    pos *= resolution;
    float d = distance(gl_FragCoord.xy, pos);
    //gl_FragColor = vec4(cos(time/3000), cos(time/5000), cos(time/1000),0.0);
   // gl_FragColor.rgb += vec3(cos(time/3000), cos(time/5000), cos(time/1000));
    gl_FragColor.rgb += smoothstep(size, 0.0, d);
    
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
    if (sceltaFS==0)
    {
        gl_FragColor = vec4(ourColor);
    }

    if (sceltaFS==1)
    {
        gl_FragColor = vec4(0.0, 0.0, 0.0, 1.0);
        vec2 rotation = vec2((mouse.x * 5.0 - 1.0), (mouse.y - 5.00)) * -PI;
        float xzLen = cos(rotation.y);
        vec3 dir = vec3(xzLen * sin(-rotation.x), sin(rotation.y), xzLen * cos(rotation.x));
        vec2 res = 1.0 / resolution;
        stars();
    } 
    
}
  


