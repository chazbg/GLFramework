#version 330 core
// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 vertex;
layout(location = 1) in vec3 normal;

uniform mat4 mvp;
uniform mat4 mv;

smooth out vec3 vNormal;

void main(){
    vNormal = normalize(mvp * normalize(vec4(normal,0))).xyz;
    gl_Position = mvp * vec4(vertex,1.0);
}

