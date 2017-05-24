#version 330 core
// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 vertexPosition_modelspace;
layout(location = 1) in vec3 normal;
uniform uint time;
uniform mat4 mvp;
uniform mat4 modelToWorld;
uniform mat4 depthMvp;

smooth out vec3 inColor;
smooth out vec4 shadowCoord;

void main(){
    vec3 light  = normalize(vec3(0, 0.15, 0.25));
    vec4 n      = normalize(modelToWorld * normalize(vec4(normal,0)));
    gl_Position = mvp * vec4(vertexPosition_modelspace,1.0);
    inColor     = dot(n.xyz, light) * vec3(1,0,0);
    shadowCoord = depthMvp * vec4(vertexPosition_modelspace,1.0);;
}

