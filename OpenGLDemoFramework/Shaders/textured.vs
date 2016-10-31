#version 330 core
// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 vertex;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 uv;

uniform mat4 mvp;
uniform mat4 mv;

smooth out vec3 vNormal;
smooth out vec2 vUV;

void main(){
    vNormal = normalize(mvp * normalize(vec4(normal,0))).xyz;
    vUV     = uv;
    gl_Position = mvp * vec4(vertex,1.0);
}

