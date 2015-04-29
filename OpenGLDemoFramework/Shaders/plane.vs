#version 330 core
// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 vertexPosition_modelspace;
uniform uint time;

smooth out vec3 inColor;

void main(){
    gl_Position.xyz = vertexPosition_modelspace;
    gl_Position.w = 1.0;

    inColor.xyz = vec3(1.0, 1.0, 0.0);
}

