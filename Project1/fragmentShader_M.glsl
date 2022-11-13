#version 330 core
in vec4 ourColor;
out vec4 FragColor;  
 
uniform int sceltaFS;
uniform float time;
uniform vec2 res;
uniform vec2 mouse;
#define PI 3.1415



void main()
{
        FragColor = vec4(ourColor);

}