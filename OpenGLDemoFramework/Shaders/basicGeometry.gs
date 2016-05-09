#version 400
layout(triangles) in;
layout(line_strip, max_vertices = 5) out;

in vec3 vNormal[];

void main()
{
    vec4 n = vec4(normalize(vNormal[0]), 0);
    vec4 t = normalize(gl_in[1].gl_Position - gl_in[0].gl_Position);
    //vec4 t = vec4(cross(normalize(gl_in[1].gl_Position - gl_in[0].gl_Position).xyz, n.xyz), 0);
    
    gl_Position = gl_in[0].gl_Position;
    EmitVertex();
    
    gl_Position = gl_in[0].gl_Position + n;
    EmitVertex();
    
    gl_Position = gl_in[0].gl_Position + 0.7 * n + 0.2 * t;
    EmitVertex();
    
    gl_Position = gl_in[0].gl_Position + 0.7 * n - 0.2 * t;
    EmitVertex();
    
    gl_Position = gl_in[0].gl_Position + n;
    EmitVertex();
    
    EndPrimitive();
}