#version 330

uniform mat4 VxProjMat;
uniform mat4 VxViewMat;

in vec3 VxVertex;
in vec3 VxNormal;

out vec3 VvNormal;

void main()
{
    VvNormal = VxNormal;

    gl_Position = VxProjMat * VxViewMat * vec4(VxVertex, 1.0);    
}