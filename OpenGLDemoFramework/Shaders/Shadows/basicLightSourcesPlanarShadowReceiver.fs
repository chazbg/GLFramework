#version 330 core

// Input data
smooth in vec3 vNormal;
smooth in vec3 vPos;
smooth in vec2 vUV;

// Ouput data
out vec3 outColor;

uniform vec3  diffuse;
uniform vec3  pointLightPos;
uniform vec3  dirLightDirection;
uniform vec3  spotLightPos;
uniform vec3  spotLightDir;
uniform float spotLightAngle;
uniform sampler2D shadowTexture;

float getPointLightContribution(vec3 n, vec3 pos, vec3 lightPos, float intensity)
{
    vec3  ray              = pos - lightPos;
    float rayLengthSquared = dot(ray, ray);
    vec3  l                = -ray / sqrt(rayLengthSquared);
    return max(0.0, dot(n, l)) * intensity / rayLengthSquared;
}

float getDirLightContribution(vec3 n, vec3 lightDir, float intensity)
{
    return max(0.0, dot(n, -lightDir)) * intensity;
}

float getSpotlightContribution(vec3 n, vec3 pos, vec3 lightPos, vec3 lightDir, float angle, float intensity)
{
    vec3  ray              = pos - lightPos;
    float rayLengthSquared = dot(ray, ray);
    vec3  l                = -ray / sqrt(rayLengthSquared);
    
    if (cos(angle) > dot(lightDir, -l))
    {
        return 0.0;
    }
    else
    {
        return max(0.0, dot(n, l)) * intensity / rayLengthSquared;
    }
}

float getMaxDistanceAtRow(sampler2D tex, vec2 uv, float row)
{
    vec2 delta = 1.0 / vec2(1600.0, 900.0);
    float d = 0.0;
    
    d = max(d, texture(tex, uv + vec2(-2.0 * delta.x, row * delta.y)).a);
    d = max(d, texture(tex, uv + vec2(-1.0 * delta.x, row * delta.y)).a);
    d = max(d, texture(tex, uv + vec2( 0.0 * delta.x, row * delta.y)).a);
    d = max(d, texture(tex, uv + vec2( 1.0 * delta.x, row * delta.y)).a);
    d = max(d, texture(tex, uv + vec2( 2.0 * delta.x, row * delta.y)).a);
    
    return d;
}

float getDistance(sampler2D tex, vec2 uv)
{
    float d = 0.0;
    
    d = max(d, getMaxDistanceAtRow(tex, uv, -2.0));
    d = max(d, getMaxDistanceAtRow(tex, uv, -1.0));
    d = max(d, getMaxDistanceAtRow(tex, uv,  0.0));
    d = max(d, getMaxDistanceAtRow(tex, uv,  1.0));
    d = max(d, getMaxDistanceAtRow(tex, uv,  2.0));
    
    return d * 10.0;
}

float blurShadow(sampler2D tex, vec2 uv, float strength)
{
    vec2 delta = 1.0 / vec2(1600.0, 900.0) * strength;
    float c = texture(tex, uv).r * 41.0 / 273.0;
    
    c += texture(tex, uv + vec2(delta.x, 0.0)).r  * 26.0 / 273.0;
    c += texture(tex, uv + vec2(0.0, delta.y)).r  * 26.0 / 273.0;
    c += texture(tex, uv + vec2(-delta.x, 0.0)).r * 26.0 / 273.0;
    c += texture(tex, uv + vec2(0.0, -delta.y)).r * 26.0 / 273.0;
                 
    c += texture(tex, uv + vec2(delta.x, delta.y)).r   * 16.0 / 273.0;
    c += texture(tex, uv + vec2(delta.x, -delta.y)).r  * 16.0 / 273.0;
    c += texture(tex, uv + vec2(-delta.x, delta.y)).r  * 16.0 / 273.0;
    c += texture(tex, uv + vec2(-delta.x, -delta.y)).r * 16.0 / 273.0;
                 
    c += texture(tex, uv + vec2(2.0 * delta.x, 0.0)).r  * 7.0 / 273.0;
    c += texture(tex, uv + vec2(0.0, 2.0 * delta.y)).r  * 7.0 / 273.0;
    c += texture(tex, uv + vec2(2.0 * -delta.x, 0.0)).r * 7.0 / 273.0;
    c += texture(tex, uv + vec2(0.0, 2.0 * -delta.y)).r * 7.0 / 273.0;
                 
    c += texture(tex, uv + vec2(2.0 * delta.x, delta.y)).r   * 4.0 / 273.0;
    c += texture(tex, uv + vec2(2.0 * delta.x, -delta.y)).r  * 4.0 / 273.0;
    c += texture(tex, uv + vec2(2.0 * -delta.x, delta.y)).r  * 4.0 / 273.0;
    c += texture(tex, uv + vec2(2.0 * -delta.x, -delta.y)).r * 4.0 / 273.0;
    c += texture(tex, uv + vec2(delta.x,  2.0 * delta.y)).r  * 4.0 / 273.0;
    c += texture(tex, uv + vec2(delta.x,  2.0 * -delta.y)).r * 4.0 / 273.0;
    c += texture(tex, uv + vec2(-delta.x, 2.0 * delta.y)).r  * 4.0 / 273.0;
    c += texture(tex, uv + vec2(-delta.x, 2.0 * -delta.y)).r * 4.0 / 273.0;
                 
    c += texture(tex, uv + vec2(2.0 * delta.x, 2.0 * delta.y)).r   * 1.0 / 273.0;
    c += texture(tex, uv + vec2(2.0 * delta.x, 2.0 * -delta.y)).r  * 1.0 / 273.0;
    c += texture(tex, uv + vec2(2.0 * -delta.x, 2.0 * delta.y)).r  * 1.0 / 273.0;
    c += texture(tex, uv + vec2(2.0 * -delta.x, 2.0 * -delta.y)).r * 1.0 / 273.0;
    
    return c;
}

vec2 poissonDisk[16] = vec2[]( 
   vec2( -0.94201624, -0.39906216 ), 
   vec2( 0.94558609, -0.76890725 ), 
   vec2( -0.094184101, -0.92938870 ), 
   vec2( 0.34495938, 0.29387760 ), 
   vec2( -0.91588581, 0.45771432 ), 
   vec2( -0.81544232, -0.87912464 ), 
   vec2( -0.38277543, 0.27676845 ), 
   vec2( 0.97484398, 0.75648379 ), 
   vec2( 0.44323325, -0.97511554 ), 
   vec2( 0.53742981, -0.47373420 ), 
   vec2( -0.26496911, -0.41893023 ), 
   vec2( 0.79197514, 0.19090188 ), 
   vec2( -0.24188840, 0.99706507 ), 
   vec2( -0.81409955, 0.91437590 ), 
   vec2( 0.19984126, 0.78641367 ), 
   vec2( 0.14383161, -0.14100790 ) 
);

// Returns a random number based on a vec3 and an int.
float random(vec3 seed, int i)
{
	vec4 seed4        = vec4(seed, i);
	float dot_product = dot(seed4, vec4(12.9898, 78.233, 45.164, 94.673));
	return fract(sin(dot_product) * 43758.5453);
}

void main()
{
    vec3 n   = normalize(vNormal);
    float c1 = getPointLightContribution(n, vPos, pointLightPos, 32.0);
    float c2 = getDirLightContribution(n, dirLightDirection, 0.25);
    float c3 = getSpotlightContribution(n, vPos, spotLightPos, spotLightDir, spotLightAngle, 16.0);
    
    float visibility = 1.0;
    vec2 delta       = 1.0 / vec2(1600.0, 900.0);
    for (int i = 0; i < 4; i++)
    {
        int   index             = int(16.0 * random(floor(vPos * 1000.0), i)) % 16;
        float projectedDistance = getDistance(shadowTexture, vUV + poissonDisk[index] * delta);
        visibility             -= 0.2 * (blurShadow(shadowTexture, vUV + poissonDisk[index] * delta, projectedDistance));
	}

    outColor = diffuse * (c1 + c2 + c3) * visibility;
}