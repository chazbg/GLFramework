#version 330 core
// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 vertex;
layout(location = 1) in vec3 normal;

uniform mat4 mvp;
uniform mat4 mv;
uniform mat4 modelToWorld;
uniform mat4 viewProjection;

uniform vec3 dirLightDirection;
uniform vec3 planeNormal;
uniform vec3 planePoint;

out float projectedDistance;

void main()
{
    vec3  v     = vec3(modelToWorld * vec4(vertex, 1.0));
    vec3  ray   = dirLightDirection;
    float t     = dot(planePoint - v, planeNormal) / dot(ray, planeNormal);
    vec3  p     = v + t * ray;
    gl_Position = viewProjection * vec4(p, 1.0);
    projectedDistance = t;
}

