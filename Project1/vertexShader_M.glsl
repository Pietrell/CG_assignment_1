#version 330 core
layout (location = 0) in vec3 aPos;   // the position variable has attribute position 0
layout (location = 1) in vec4 aColor; // the color variable has attribute position 1
  
out vec4 ourColor; 
uniform mat4 Projection;  
uniform mat4 Model;
uniform float time;

void main() {
     gl_Position = Projection*Model*vec4(aPos, 1.0);
    ourColor = aColor;
} 