#version 330 core
// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 vertexPosition_modelspace;
layout(location = 1) in vec3 normal;

uniform mat4 mvp;
uniform mat4 mv;

out vec4 n;

void main(){
    n = normalize(mv * normalize(vec4(normal,0)));
    gl_Position = mvp * vec4(vertexPosition_modelspace,1.0);
}

