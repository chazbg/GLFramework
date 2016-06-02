#version 330 core

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec2 vertexPosition_modelspace;
layout(location = 2) in vec2 texCoord;

smooth out vec2 texCoords;
flat out float alpha;

uniform float remainingLife;
uniform float duration;
uniform float tangentAcceleration;
uniform float radialAcceleration;
uniform vec2 scale;
uniform float phase;
 
void main(){
    const float MAX_TANGENT_VELOCITY = 0.0f;
    const float MAX_RADIAL_VELOCITY = 0.0f;
    const vec2 emitterPos = vec2(0.0f);
    
    float normalizedLife = remainingLife / duration;
    float normalizedElapsedLife = 1.0f - normalizedLife;
    float elapsedLife = duration - remainingLife;
    gl_Position = vec4(vertexPosition_modelspace.xy * scale, 0, 1);

	float elaspedLifeSq = elapsedLife * elapsedLife * 0.5;
    float t1 = elaspedLifeSq * tangentAcceleration;
    float t2 = elaspedLifeSq * radialAcceleration * 5.0;
    
    gl_Position.x += t2 * 0.005; 
    float cosTheta = cos(t1);
    float sinTheta = sin(t1); // sqrt(1.0 - cosTheta * cosTheta);
    mat2 rotation = mat2(vec2(cosTheta, -sinTheta),
                         vec2(sinTheta, cosTheta));
    gl_Position.xy *= rotation;
    
    alpha = normalizedLife;  
    
    texCoords = texCoord;
}

