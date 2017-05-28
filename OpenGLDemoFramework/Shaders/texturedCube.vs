#version 330 core
// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 uv;

uniform mat4 mvp;
uniform mat4 modelToWorld;
uniform mat4 depthMvp;
uniform mat4 depthModelView;

smooth out vec3 inNormal;
smooth out vec4 shadowCoord;
smooth out vec2 inUVs;
smooth out vec4 lightViewPos;

void main()
{
    vec4 n       = normalize(modelToWorld * normalize(vec4(normal, 0.0)));
    gl_Position  = mvp * vec4(pos, 1.0);
	inNormal     = n.xyz;
    shadowCoord  = depthMvp * vec4(pos, 1.0);
    inUVs        = uv;
    lightViewPos = depthModelView * vec4(pos, 1.0);
}

