#version 330 core

layout(location = 0) in vec3 vertexPosition_modelspace;
 
uniform mat4 mvp;
uniform uint time;

void main()
{
    float step = 0.01;
    float thetaRot = time * step;
    mat4 rotY;
    rotY[0] = vec4(cos(thetaRot), 0, sin(thetaRot), 0);
    rotY[1] = vec4(0, 1, 0, 0);
    rotY[2] = vec4(-sin(thetaRot), 0, cos(thetaRot), 0);
    rotY[3] = vec4(0, 0, 0, 1);
    
    mat4 scale;
    scale[0] = vec4(0.7,0,0,0);
    scale[1] = vec4(0,0.7,0,0);
    scale[2] = vec4(0,0,0.7,0);
    scale[3] = vec4(0,0,0,1);
    
    vec4 transl;
    //transl = vec4(cos(thetaRot), 0.0, 3.0+sin(thetaRot),0);
    //transl = vec4(-0.5, -0.5, 0,0);
    //transl = vec4(0, 0, -0.7, 0);
    gl_Position = mvp * transpose(rotY) * vec4(vertexPosition_modelspace,1.0);
}