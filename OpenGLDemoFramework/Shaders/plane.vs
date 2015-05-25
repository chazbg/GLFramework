#version 330 core
// Input vertex data, different for all executions of this shader.
in vec3 vertexPosition_modelspace;
in vec3 normal;
uniform uint time;

smooth out vec4 inColor;
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

struct lightSampleValues {
	vec3 L;
	float iL;
};

vec3 computeAmbientComponent(lightSampleValues light, vec3 materialAmbientColor, vec3 lightAmbDiffSpec, vec3 lightColor)
{
	return light.iL * (lightColor * lightAmbDiffSpec.x) * materialAmbientColor;
}

// surfaceNormal is assumed to be unit-length
vec3 computeDiffuseComponent(vec3 surfaceNormal, lightSampleValues light, vec3 materialDiffuseColor, vec3 lightAmbDiffSpec, vec3 lightColor)
{
	return light.iL * (lightColor * lightAmbDiffSpec.y)
					* materialDiffuseColor.rgb
					* max(0.0, dot(surfaceNormal, light.L));
}

vec3 computeSpecularComponent(vec3 surfaceNormal, vec4 surfacePosition, lightSampleValues light, vec3 materialSpecularColor, float materialSpecularExp, vec3 lightAmbDiffSpec, vec3 lightColor)
{
	vec3 viewVector = normalize(-surfacePosition.xyz);
	vec3 reflectionVector = 2.0 * dot(light.L, surfaceNormal) * surfaceNormal - light.L;
	return (dot(surfaceNormal, light.L) <= 0.0) ? vec3(0.0,0.0,0.0) : (light.iL * (lightColor * lightAmbDiffSpec.z)
																				* materialSpecularColor
																				* pow(max(0.0, dot(reflectionVector, viewVector)), materialSpecularExp));
}

lightSampleValues computeDirLightValues(vec3 dirLightPosition, float dirLightIntensity)
{
	lightSampleValues values;
	values.L = dirLightPosition.xyz;
	values.iL = dirLightIntensity;
	return values;
}

lightSampleValues computePointLightValues(vec3 pointLightPosition, vec3 pointLightAttenuation, float pointLightIntensity, vec4 surfacePosition)
{
	lightSampleValues values;
	values.L = pointLightPosition.xyz - surfacePosition.xyz;
	float dist = length(values.L);
	values.L = values.L / dist; // normalize
	// Dot computes the 3-term attenuation in one operation
	// k_c * 1.0 + k_l * dist + k_q * dist * dist
	float distAtten = dot(pointLightAttenuation, vec3(1.0, dist, dist*dist));
	values.iL = pointLightIntensity / distAtten;
	return values;
}

lightSampleValues computeSpotLightValues(vec3 spotLightPosition, 
	vec3 spotLightAttenuation, 
	vec3 spotLightDir, 
	float spotLightAngleCos, 
	float spotLightExponent,
	float spotLightIntensity,
	vec4 surfacePosition)
{
	lightSampleValues values;
	values.L = spotLightPosition.xyz - surfacePosition.xyz;
	float dist = length(values.L);
	values.L = values.L / dist; // normalize
	// Dot computes the 3-term attenuation in one operation
	// k_c * 1.0 + k_l * dist + k_q * dist * dist
	float distAtten = dot(spotLightAttenuation,	vec3(1.0, dist, dist*dist));
	float spotAtten = dot(-spotLightDir, values.L);
	spotAtten = (spotAtten > spotLightAngleCos) ? pow(spotAtten, spotLightExponent) : 0.0;
	values.iL = spotLightIntensity * spotAtten / distAtten;
	return values;
}

void main(){
	vec3 ambColor = vec3(1, 1, 0);
	vec3 emissiveColor = vec3(1,1,1);
	vec4 diffuseColor = vec4(1,0,0,1);
	vec3 specularColor = vec3(1,1,1);
	vec3 lightAmbDiffSpec = vec3(0.02,0.68,0.3);
	vec3 lightColor = vec3(1,1,1);
	float specExp = 4.0;
    
    float angle = radians(20.0);
    mat4 rot;
    rot[0] = vec4(1, 0, 0, 0);
    rot[1] = vec4(0, cos(angle), sin(angle), 0);
    rot[2] = vec4(0, -sin(angle), cos(angle), 0);
    rot[3] = vec4(0, 0, 0, 1);
    rot = transpose(rot);
    vec3 normal = getNormal(vertexPosition_modelspace * 0.02 + vec3(-0.5, -0.5, 0));
    vec3 n = (rot * vec4(normal,0)).xyz;

    gl_Position.xyz = toWaves(vertexPosition_modelspace * 0.02 + vec3(-0.5, -0.5, 0));
    gl_Position.w = 1.0;
    gl_Position = rot * gl_Position;
    
    lightSampleValues light = computePointLightValues(vec3(0, 0, 1), vec3(0,0,1), 1, gl_Position);
    //lightSampleValues light = computeDirLightValues(vec3(0, 0, 1), 1);
    
    vec3 ambComp = computeAmbientComponent(light, ambColor, lightAmbDiffSpec, lightColor);
	vec3 diffComp = computeDiffuseComponent(n, light, diffuseColor.xyz, lightAmbDiffSpec, lightColor);
	vec3 specComp = computeSpecularComponent(n, gl_Position, light, specularColor, specExp, lightAmbDiffSpec, lightColor);
    
    inColor.xyz = ambComp + diffComp + specComp;
    inColor.w = diffuseColor.w;
}

