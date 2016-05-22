#version 330 core
// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 vertex;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 uvs;

uniform mat4 mvp;
uniform mat4 modelToWorld;

smooth out vec3 vNormal;
smooth out vec3 lightDir;
smooth out vec2 vUVs;

void main(){
    lightDir = normalize(modelToWorld * normalize(vec4(0,1,1,0))).xyz;
    vNormal = normalize(modelToWorld * normalize(vec4(normal,0))).xyz;
    vUVs = uvs;
    gl_Position = mvp * vec4(vertex,1.0);
}

