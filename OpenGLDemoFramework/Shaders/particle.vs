#version 330 core

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec2 vertexPosition_modelspace;
layout(location = 1) in vec2 texCoord;

out vec2 texCoords;
out vec2 interpolatedCoords;

uniform uint time;
uniform mat4 mvp;

void main(){
    gl_Position = vec4((mvp * vec4(vertexPosition_modelspace, 0, 1)).xy, 0, 1);
    interpolatedCoords = gl_Position.xy;
    texCoords = texCoord;
}

