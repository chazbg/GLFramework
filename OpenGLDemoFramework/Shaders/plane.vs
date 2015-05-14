#version 330 core
// Input vertex data, different for all executions of this shader.
in vec3 vertexPosition_modelspace;
in vec3 normal;
uniform uint time;

smooth out vec3 inColor;
vec3 toWave(vec3 p, float amplitude, float waveLength, float speed, vec2 direction)
{
    float frequency = 2 * 3.14 / waveLength;
    float phase = speed * frequency;
    
    return vec3(p.xy, amplitude * sin(dot(direction, p.xy) * frequency + 0.01 * float(time) * phase));
}

vec3 toWaves(vec3 point)
{
    vec3 w1 = toWave(point, 2, 1, 0.5, vec2(1,0));
    vec3 w2 = toWave(point, 1, 1.2, 0.5, vec2(1,0));
    vec3 w3 = toWave(point, 1.5, 1.2, 0.5, vec2(0,1));
    return w1 + w2 + w3;
}

void main(){
    float angle = 3.14 / 4.0;
    mat4 rot;
    rot[0] = vec4(1, 0, 0, 0);
    rot[1] = vec4(0, cos(angle), sin(angle), 0);
    rot[2] = vec4(0, -sin(angle), cos(angle), 0);
    rot[3] = vec4(0, 0, 0, 1);
    
    gl_Position.xyz = toWaves(vertexPosition_modelspace) * 0.02 - vec3(1, 1, 0.0);
    gl_Position.w = 1.0;
    gl_Position = rot * gl_Position;
    inColor.xyz = normal;
}

