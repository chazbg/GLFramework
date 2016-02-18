#version 330 core
// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 vertex;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 uv;
layout(location = 3) in vec3 tangent;

uniform mat4 mvp;
uniform mat4 modelToWorld;
uniform mat4 modelView;
uniform mat4 depthMvp;

smooth out vec3 pos;
smooth out vec3 inNormal;
smooth out vec3 inTangent;
smooth out vec3 inBitangent;
smooth out vec2 inUVs;

smooth out vec4 shadowCoord;

void main(){
    mat4 shadowBias;
    shadowBias[0] = vec4(0.5,0,0,0);
    shadowBias[1] = vec4(0,0.5,0,0);
    shadowBias[2] = vec4(0,0,0.5,0);
    shadowBias[3] = vec4(0.5,0.5,0.5,1);

    pos         = vec3(modelToWorld * vec4(vertex, 1.0));
    inNormal    = vec3(modelToWorld * normalize(vec4(normal, 0)));
    inTangent   = vec3(modelToWorld * normalize(vec4(tangent, 0)));
    inBitangent = cross(inNormal, inTangent);
    inUVs       = uv;
    
    shadowCoord = shadowBias * depthMvp * vec4(vertex, 1.0);
    
    gl_Position = mvp * vec4(vertex, 1.0);
}

