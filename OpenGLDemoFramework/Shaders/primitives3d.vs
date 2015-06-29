#version 330 core

in vec3 position;

uniform vec3 color;
uniform float pointSize;

out vec3 interpolatedColor;

void main()
{
    mat4 proj;
    
    // proj[0] = vec4(1, 0, 0, 1);
    // proj[1] = vec4(0, 1, 0, 1);
    // proj[2] = vec4(0, 0, 0, 0);
    // proj[3] = vec4(0, 0, 1, 2);
    
    proj[0] = vec4(1, 0, -0.35, 1);
    proj[1] = vec4(0, 1, -0.35, 0);
    proj[2] = vec4(0, 0, 0, 0);
    proj[3] = vec4(0, 0, 0, 1);
    
    gl_PointSize = pointSize;
    interpolatedColor = color;
    if (position.z > 0)
    {
        interpolatedColor = vec3(1, 0, 0);
    }
    gl_Position = transpose(proj) * vec4(position, 1);
    gl_Position.xyz *= 0.3f;
}

