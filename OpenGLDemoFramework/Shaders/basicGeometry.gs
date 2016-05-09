#version 400
layout(triangles) in;
layout(line_strip, max_vertices = 6) out;

in vec3 vNormal[];

void main()
{
    for (int i = 0; i < 3; i++)
    {
        gl_Position = gl_in[i].gl_Position;
        EmitVertex();
        
        gl_Position = gl_in[i].gl_Position + vec4(normalize(vNormal[i]), 0);
        EmitVertex();
        
        EndPrimitive();
    }
}