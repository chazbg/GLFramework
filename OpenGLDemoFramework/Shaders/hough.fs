#version 330 core
uniform uint time;
in vec2 interpolatedCoords;
in vec2 texCoords;

// Ouput data
out vec3 color;

uniform sampler2D sampler;

void main()
{
    float delta = 1 / 200.0f;
    vec2 rTheta = texCoords;
    rTheta.x *= 3.14;
    
    float r = rTheta.y;
    float sinTheta = sin(rTheta.x);
    float cosTheta = cos(rTheta.x);
    
    //vec2 A = vec2(0, -r / sinTheta);
    //vec2 B = vec2(-r / cosTheta, 0);
    //vec2 AB = B - A;
    
    color = vec3(0);
    
    // for (float i = 0; i < 1; i += delta)
    // {
        // vec2 p = A + AB * i;
 
            // color += vec3(texture(sampler, vec2(p)).b);
    // }
    
    for (float i = 0; i < 1; i += delta)
    {
        for (float j = 0; j < 1; j += delta)
        {
            vec2 p = vec2(i, j);
            if (abs(r - (i*cosTheta + j*sinTheta)) < 0.003)
            {
                color += vec3(texture(sampler, p).b);
            }
        }
    }
    
    //color = log(color) / 4;
    color /= 25.0f;  
    //color = texture(sampler, texCoords).bgr;      
}