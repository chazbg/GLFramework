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
    
    gl_Position = vec4(vertexPosition_modelspace.xy * scale, 0, 1);

    float t1 = normalizedElapsedLife * tangentAcceleration * 200.0f;
    float t2 = normalizedElapsedLife * radialAcceleration  * 1000.0f;
    vec2 newPos = vec2(cos(t1), sin(t1)) * t2;
    
    gl_Position.xy += newPos; 
    // vec2 radialDir = normalize(gl_Position.xy - emitterPos);
	// vec2 tangentDir = vec2(radialDir.y, -radialDir.x);
    // float tangentVelocity = min(MAX_TANGENT_VELOCITY, tangentAcceleration);
    // float radialVelocity = min(MAX_RADIAL_VELOCITY, radialAcceleration);
    // gl_Position.xy += tangentVelocity * tangentDir * (1.0 - normalizedLife) + 
                      // radialVelocity  * radialDir  * (1.0 - normalizedLife);
    alpha = normalizedLife;  
    
    texCoords = texCoord;
}

