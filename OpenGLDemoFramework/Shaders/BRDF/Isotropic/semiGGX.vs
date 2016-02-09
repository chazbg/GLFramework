#version 330 core
// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 vertexPosition_modelspace;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 uv;
layout(location = 3) in vec3 tangent;

uniform mat4 mvp;
uniform mat4 mv;
uniform mat4 modelview;
uniform mat4 depthMvp;

smooth out vec3 inNormal;
smooth out vec4 shadowCoord;
smooth out vec2 inUVs;
smooth out vec3 pos;
smooth out vec3 posV;
smooth out vec3 inTangent;
smooth out vec3 inBitangent;

void main(){
    mat4 shadowBias;
    shadowBias[0] = vec4(0.5,0,0,0);
    shadowBias[1] = vec4(0,0.5,0,0);
    shadowBias[2] = vec4(0,0,0.5,0);
    shadowBias[3] = vec4(0.5,0.5,0.5,1);

    inNormal = vec3(normalize(mv * normalize(vec4(normal,0))));
    inTangent = vec3(normalize(mv * normalize(vec4(tangent,0))));
    inBitangent = cross(inNormal, inTangent);
    pos = vec3(mv * vec4(vertexPosition_modelspace, 1.0));
    posV = vec3(modelview * vec4(vertexPosition_modelspace, 1.0));
    inUVs = uv;
    shadowCoord = shadowBias * depthMvp * vec4(vertexPosition_modelspace,1.0);
    gl_Position = mvp * vec4(vertexPosition_modelspace,1.0);
}

