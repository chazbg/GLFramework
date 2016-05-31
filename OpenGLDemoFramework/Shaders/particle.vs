#version 330 core

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec2 vertexPosition_modelspace;
layout(location = 2) in vec2 texCoord;

smooth out vec2 texCoords;
flat out float alpha;

uniform uint time;
uniform mat4 mvp;
uniform float remainingLife;
uniform float duration;
uniform float tangentAcceleration;
uniform float radialAcceleration;
uniform vec2 scale;

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
    vec2 newPos = vec2(cos(t1), sin(t1)) * t2;
    
    gl_Position.xy += newPos; 
	
    alpha = normalizedLife;  
    
    texCoords = texCoord;
}

