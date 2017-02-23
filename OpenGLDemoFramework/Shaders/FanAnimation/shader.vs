#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;

uniform mat4 modelToWorld;
uniform mat4 modelView;
uniform mat4 mvp;

uniform float index;
uniform float wingCount;
uniform float time;

smooth out vec3 vNormal;
flat   out float vOpacity;

const float PI        = 3.14159265;
const float DOUBLE_PI = 6.2831853;

vec3 rotateAroundY(float t, float initialAngle, float endAngle, vec3 pos)
{
    float theta = mix(initialAngle, endAngle, t);
    mat3  rot   = mat3(vec3( cos(theta), 0.0, sin(theta)),
                       vec3( 0.0,        1.0, 0.0),
                       vec3(-sin(theta), 0.0, cos(theta)));
    return rot * pos;
}

vec3 rotateAroundZ(float t, float initialAngle, float endAngle, vec3 pos)
{
    float theta = (-PI / wingCount) + (-DOUBLE_PI * index / wingCount) + mix(initialAngle, endAngle, t);
    mat3  rot   = mat3(vec3(cos(theta), -sin(theta), 0.0),
                       vec3(sin(theta), cos(theta),  0.0),
                       vec3(0.0,        0.0,         1.0));
    return rot * pos;
}

vec3 scaleXY(float t, float initialScale, float endScale, vec3 pos)
{
    float scale = mix(initialScale, endScale, t);
    return vec3(pos.xy * scale, pos.z);
}

float calculateOpacity(float t, float initialValue, float endValue)
{
    return mix(initialValue, endValue, t);
}

void main()
{
    gl_Position.xyz = position;    
    
    float offset    = index * 0.033;
    float tRotY     = (clamp(time,         offset, offset + 0.133) - offset) / 0.133;
    float tScaleXY  = (clamp(time - 0.033, offset, offset + 0.267) - offset) / 0.267;
    float tRotZ     = (clamp(time - 0.033, offset, offset + 0.467) - offset) / 0.467;
    float tOpacity  = (clamp(time - 0.200, offset, offset + 0.100) - offset) / 0.100;
               
    gl_Position.xyz = rotateAroundY(tRotY, 0.0, -PI * 0.25,      position);
    gl_Position.xyz = scaleXY(tScaleXY,    1.0, 0.3,             gl_Position.xyz);
    gl_Position.xyz = rotateAroundZ(tRotZ, 0.0, -PI * 2.0 / 3.0, gl_Position.xyz);
    gl_Position.w   = 1.0;
    
    vec3 n = rotateAroundY(tRotY, 0.0, -PI * 0.25,      normal);
         n = rotateAroundZ(tRotZ, 0.0, -PI * 2.0 / 3.0, n);
         
    gl_Position     = mvp * gl_Position;
    vNormal         = vec3(modelToWorld * normalize(vec4(n, 0)));
    vOpacity        = calculateOpacity(tOpacity, 1.0, 0.0);
}