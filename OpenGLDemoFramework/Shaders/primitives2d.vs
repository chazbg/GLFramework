#version 330 core

in vec2 position;

uniform vec3 color;
uniform float pointSize;

out vec3 interpolatedColor;

void main()
{
    gl_PointSize = pointSize;
    gl_Position.xy = position * 0.05 + vec2(-0.5, -0.5);
    gl_Position.z = 0.0;
    gl_Position.w = 1.0;
    interpolatedColor = color;
}

