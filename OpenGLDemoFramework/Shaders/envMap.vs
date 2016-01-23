#version 330 core
// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 vertex;

uniform mat4 mvp;

smooth out vec3 iPos;

void main(){
    gl_Position = mvp * vec4(vertex,1.0);
    iPos = vertex;
}

