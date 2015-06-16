#version 330 core
// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 vertexPosition_modelspace;
layout(location = 1) in vec3 normal;
uniform uint time;

smooth out vec3 inColor;

mat4 view_frustum(
    float angle_of_view,
    float aspect_ratio,
    float z_near,
    float z_far
) {
    return mat4(
        vec4(1.0/tan(angle_of_view),           0.0, 0.0, 0.0),
        vec4(0.0, aspect_ratio/tan(angle_of_view),  0.0, 0.0),
        vec4(0.0, 0.0,    (z_far+z_near)/(z_far-z_near), 1.0),
        vec4(0.0, 0.0, -2.0*z_far*z_near/(z_far-z_near), 0.0)
    );
}

void main(){
    vec3 light = vec3(-1,-1,-1);
    float step = 0.01;
    float thetaRot = time * step;
    mat4 rotation;
    vec4 pos = vec4(vertexPosition_modelspace,1);
    mat4 proj;
    mat4 rotY, rotX;
    rotY[0] = vec4(cos(thetaRot), 0, sin(thetaRot), 0);
    rotY[1] = vec4(0, 1, 0, 0);
    rotY[2] = vec4(-sin(thetaRot), 0, cos(thetaRot), 0);
    rotY[3] = vec4(0, 0, 0, 1);
    
    rotX[0] = vec4(1, 0, 0, 0);
    rotX[1] = vec4(0, cos(3.14/6.0), sin(3.14/6.0), 0);
    rotX[2] = vec4(0, -sin(3.14/6.0), cos(3.14/6.0), 0);
    rotX[3] = vec4(0, 0, 0, 1);
    
    // proj[0] = vec4(-1.0/sqrt(2.0), 1.0/sqrt(2.0), 0, 0);
    // proj[1] = vec4(-1.0/sqrt(6.0), -1.0/sqrt(6.0), -sqrt(2.0)/sqrt(3.0), 0);
    // proj[2] = vec4(0, 0, 0, 0);
    // proj[3] = vec4(0, 0, 0, 1);
    
    // proj[0] = vec4(1, 0, 1.0/sqrt(2.0), 0);
    // proj[1] = vec4(0,  1, 1.0/sqrt(2.0), 0);    
    // proj[2] = vec4(0, 0, 0, 0);
    // proj[3] = vec4(0, 0, 0, 1);
    
	float fov = radians(45.0);
	float near = 0.5;
	float far = 5;
	float aspect = 2.0;
	float f = 1.0 / tan(fov / 2.0);
	proj = view_frustum(fov, aspect, near, far);
	// proj[0] = vec4(f/aspect, 0, 0, 0);
    // proj[1] = vec4(0, f, 0, 0);
    // proj[2] = vec4(0, 0, (far + near) / (near - far), (2 * far * near) / (near - far));
    // proj[3] = vec4(0, 0, 1, 0);
	
    mat4 scale;
    scale[0] = vec4(0.2,0,0,0);
    scale[1] = vec4(0,0.2,0,0);
    scale[2] = vec4(0,0,0.2,0);
    scale[3] = vec4(0,0,0,1);
    vec4 transl;
    transl = vec4(cos(thetaRot), 0.0, 3.0+sin(thetaRot),0);
    
    rotation[0] = vec4(cos(thetaRot), 0, sin(thetaRot), 0);
    rotation[1] = vec4(0, 1, 0, 0);
    rotation[2] = vec4(-sin(thetaRot), 0, cos(thetaRot), 0);
    rotation[3] = vec4(0, 0, 0, 1);

    vec4 n = rotation * vec4(normal,1);
    gl_Position = proj * (transl + transpose(rotY) * vec4(vertexPosition_modelspace,1.0));
    inColor = vec3(dot(n.xyz, light)*0.5,0,0);
}

