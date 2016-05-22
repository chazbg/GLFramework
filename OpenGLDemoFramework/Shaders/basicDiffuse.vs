#version 330 core
// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 vertex;
layout(location = 1) in vec3 normal;

uniform mat4 mvp;
uniform mat4 mv;

smooth out vec3 vNormal;
smooth out vec3 lightDir;

void main(){
    lightDir = normalize(mvp * normalize(vec4(0,1,1,0))).xyz;
    vNormal = normalize(mvp * normalize(vec4(normal,0))).xyz;
    gl_Position = mvp * vec4(vertex,1.0);
}

