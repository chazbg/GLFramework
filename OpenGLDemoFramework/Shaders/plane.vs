#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 texCoords;
uniform uint time;
uniform mat4 mvp;
uniform vec3 cameraPos;

out vec3 inColor;
out vec2 inTexCoords;

vec3 toWave(vec3 p, float amplitude, float waveLength, float speed, vec2 direction)
{
    float frequency = 2 * 3.14 / waveLength;
    float phase = speed * frequency;
    
    return vec3(p.xy, amplitude * sin(dot(direction, p.xy) * frequency + 0.01 * float(time) * phase));
}

float toWaveDx(vec3 p, float amplitude, float waveLength, float speed, vec2 direction)
{
    float frequency = 2 * 3.14 / waveLength;
    float phase = speed * frequency;
    
    return amplitude * cos(dot(direction, p.xy) * frequency + 0.01 * float(time) * phase) * direction.x * frequency;
}

float toWaveDy(vec3 p, float amplitude, float waveLength, float speed, vec2 direction)
{
    float frequency = 2 * 3.14 / waveLength;
    float phase = speed * frequency;
    
    return amplitude * cos(dot(direction, p.xy) * frequency + 0.01 * float(time) * phase) * direction.y * frequency;
}

vec3 toWaves(vec3 point)
{
    vec3 w1 = toWave(point, 0.3, 1, 0.1, vec2(1,1));
    vec3 w2 = toWave(point, 0.5, 1, 0.4, vec2(1,0));
    vec3 w3 = toWave(point, 0.2, 1.2, 0.3, vec2(1,0));
    vec3 res = w1 + w2 + w3;
    res.x = res.x / 3.0;
    res.y = res.y / 3.0;
    return res;
}

float toWavesDx(vec3 point)
{
    float w1 = toWaveDx(point, 0.3, 1, 0.1, vec2(1,1));
    float w2 = toWaveDx(point, 0.5, 1, 0.4, vec2(1,0));
    float w3 = toWaveDx(point, 0.2, 1.2, 0.3, vec2(1,0));
    float res = w1 + w2 + w3;
    return res;
}

float toWavesDy(vec3 point)
{
    float w1 = toWaveDy(point, 0.3, 1, 0.1, vec2(1,1));
    float w2 = toWaveDy(point, 0.5, 1, 0.4, vec2(1,0));
    float w3 = toWaveDy(point, 0.2, 1.2, 0.3, vec2(1,0));
    float res = w1 + w2 + w3;
    return res;
}

vec3 getNormal(vec3 r)
{
    vec3 B = vec3(1, 0, toWavesDx(r));
    vec3 T = vec3(0, 1, toWavesDy(r));
    return normalize(cross(B, T));
}

void main()
{
	vec3 diffuseColor = vec3(1.0, 0.0, 0.0);
	float specExp     = 16.0;

    vec3 N            = getNormal(position * 0.02 + vec3(-0.5, -0.5, 0.0));
    vec3 L            = vec3(0.0, 0.0, 1.0);
    vec3 V            = normalize(cameraPos - position); 
    vec3 H            = normalize(V + L);
    inColor           = diffuseColor * max(0.0, dot(N, L)) + pow(max(0.0, dot(N, H)), specExp);
    inTexCoords       = texCoords;
    
    gl_Position.xyz   = toWaves(position * 0.02 + vec3(-0.5, -0.5, 0.0));
    gl_Position.w     = 1.0;
    gl_Position       = mvp * gl_Position;
}

