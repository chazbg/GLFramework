#version 330 core
// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 vertexPosition_modelspace;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 uv;

uniform mat4 mvp;
uniform mat4 modelToWorld;
uniform mat4 depthMvp;

smooth out vec3 inNormal;
smooth out vec4 shadowCoord;
smooth out vec2 inUVs;
smooth out vec3 pos;

void main(){
    vec3 light = normalize(vec3(0, 0.15, 0.25));

    mat4 shadowBias;
    shadowBias[0] = vec4(0.5,0,0,0);
    shadowBias[1] = vec4(0,0.5,0,0);
    shadowBias[2] = vec4(0,0,0.5,0);
    shadowBias[3] = vec4(0.5,0.5,0.5,1);

    vec4 n = normalize(modelToWorld * normalize(vec4(normal,0)));
    gl_Position = mvp * vec4(vertexPosition_modelspace,1.0);
	inNormal = n.xyz;
    //inColor = dot(n.xyz, light) * vec3(1,0,0);
    vec4 sc = shadowBias * depthMvp * vec4(vertexPosition_modelspace,1.0);
    shadowCoord = sc;
    
    inUVs = uv;
    pos = (modelToWorld * vec4(vertexPosition_modelspace, 1.0)).xyz;
}

