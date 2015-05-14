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
    vec3 w1 = toWave(point, 2, 1, 0.1, vec2(1,1));
    vec3 w2 = toWave(point, 1, 1.5, 0.2, vec2(1,0));
    vec3 w3 = toWave(point, 1.5, 1.2, 0.3, vec2(1,0));
    return w1 + w2 + w3;
}

vec3 getNormal(vec3 r)
{
    vec3 eps = vec3(0.01f, 0.0f, 0.0f);
    vec3 n = vec3(toWaves(r + eps).z - toWaves(r - eps).z,
	  			  toWaves(r + eps.yxz).z - toWaves(r - eps.yxz).z,
	  			  toWaves(r + eps.zyx).z - toWaves(r - eps.zyx).z);
    return normalize(n);
}

void main(){
    float angle = radians(-45.0);
    mat4 rot;
    rot[0] = vec4(1, 0, 0, 0);
    rot[1] = vec4(0, cos(angle), sin(angle), 0);
    rot[2] = vec4(0, -sin(angle), cos(angle), 0);
    rot[3] = vec4(0, 0, 0, 1);
    vec3 n = (rot * vec4(getNormal(vertexPosition_modelspace),0)).xyz;
    gl_Position.xyz = toWaves(vertexPosition_modelspace) * 0.02 - vec3(1, 1, 0.0);
    gl_Position.w = 1.0;
    gl_Position = rot * gl_Position;
    inColor.xyz = vec3(1,0,0) * dot(n, vec3(1,0,0));
}

