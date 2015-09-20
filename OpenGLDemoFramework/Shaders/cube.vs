#version 330 core
// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 vertexPosition_modelspace;
layout(location = 1) in vec3 normal;
uniform uint time;
uniform mat4 mvp;
uniform mat4 mv;
uniform mat4 depthMvp;

smooth out vec3 inColor;
smooth out vec4 shadowCoord;

void main(){
    vec3 light = normalize(vec3(0, 0.15, 0.25));

    mat4 shadowBias;
    shadowBias[0] = vec4(0.5,0,0,0);
    shadowBias[1] = vec4(0,0.5,0,0);
    shadowBias[2] = vec4(0,0,0.5,0);
    shadowBias[3] = vec4(0.5,0.5,0.5,1);

    vec4 n = normalize(mv * normalize(vec4(normal,0)));
    gl_Position = mvp * vec4(vertexPosition_modelspace,1.0);

    inColor = vec3(dot(n.xyz, light),0,0);
    vec4 sc = shadowBias * depthMvp * vec4(vertexPosition_modelspace,1.0);
    shadowCoord = sc;
}

