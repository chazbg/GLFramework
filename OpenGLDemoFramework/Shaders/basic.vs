#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;

uniform mat4 modelToWorld;
uniform mat4 modelView;
uniform mat4 mvp;

smooth out vec3 vNormal;

void main()
{
    gl_Position.xyz = position;
    gl_Position.w   = 1.0;
    gl_Position     = mvp * gl_Position;
    vNormal         = vec3(modelToWorld * normalize(vec4(normal, 0)));
}