#version 330 core
// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 vertexPosition_modelspace;
layout(location = 1) in vec3 normal;
uniform uint time;
uniform mat4 mvp;
uniform mat4 depthMvp;

smooth out vec3 inColor;
smooth out vec3 shadowCoord;

void main(){
    vec3 light = vec3(0, 1, 1);
    float step = 0.01;
    float thetaRot = time * step;
    mat4 rotation;
    vec4 pos = vec4(vertexPosition_modelspace,1);
    mat4 proj;
    mat4 rotY, rotX;
    rotY[0] = vec4(cos(thetaRot), 0, sin(thetaRot), 0);
    rotY[1] = vec4(0, 1, 0, 0);
    rotY[2] = vec4(-sin(thetaRot), 0, cos(thetaRot), 0);
    rotY[3] = vec4(0, 0, 0, 1);
    
    rotX[0] = vec4(1, 0, 0, 0);
    rotX[1] = vec4(0, cos(3.14/2.5), sin(3.14/2.5), 0);
    rotX[2] = vec4(0, -sin(3.14/2.5), cos(3.14/2.5), 0);
    rotX[3] = vec4(0, 0, 0, 1);
    
    // proj[0] = vec4(-1.0/sqrt(2.0), 1.0/sqrt(2.0), 0, 0);
    // proj[1] = vec4(-1.0/sqrt(6.0), -1.0/sqrt(6.0), -sqrt(2.0)/sqrt(3.0), 0);
    // proj[2] = vec4(0, 0, 0, 0);
    // proj[3] = vec4(0, 0, 0, 1);
    
    // proj[0] = vec4(1, 0, 1.0/sqrt(2.0), 0);
    // proj[1] = vec4(0,  1, 1.0/sqrt(2.0), 0);    
    // proj[2] = vec4(0, 0, 0, 0);
    // proj[3] = vec4(0, 0, 0, 1);
	
    mat4 shadowBias;
    shadowBias[0] = vec4(0.5,0,0,0);
    shadowBias[1] = vec4(0,0.5,0,0);
    shadowBias[2] = vec4(0,0,0.5,0);
    shadowBias[3] = vec4(0.5,0.5,0.5,1);
    
    mat4 scale;
    scale[0] = vec4(0.1,0,0,0);
    scale[1] = vec4(0,0.1,0,0);
    scale[2] = vec4(0,0,0.1,0);
    scale[3] = vec4(0,0,0,1);
    
    vec4 transl;
    transl = vec4(0, 0, sin(thetaRot),0);
    //transl = vec4(-0.5, -0.5, 0,0);
    //transl = vec4(0, 0, -10.0, 0);

    vec4 n = transpose(rotY) * normalize(vec4(normal,1));
    gl_Position = mvp * transpose(rotY) * vec4(vertexPosition_modelspace,1.0);
    gl_Position = gl_Position / gl_Position.w;

    inColor = vec3(dot(n.xyz, light)*0.5,0,0);
    shadowCoord = (shadowBias * depthMvp * transpose(rotY) * vec4(vertexPosition_modelspace,1.0)).xyz;
}

