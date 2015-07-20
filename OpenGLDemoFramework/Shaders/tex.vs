#version 330 core

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec2 vertexPosition_modelspace;
layout(location = 2) in vec2 texCoord;

out vec2 texCoords;
out vec2 interpolatedCoords;

void main(){

    gl_Position.xy = vertexPosition_modelspace;
    gl_Position.z = 0.0;
    gl_Position.w = 1.0;
    interpolatedCoords = vertexPosition_modelspace;
    texCoords = texCoord;
}

