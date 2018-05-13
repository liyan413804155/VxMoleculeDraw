#version 330

uniform mat4 VxProjMat;
uniform mat4 VxViewMat;

in vec3 VxVertex;

void main()
{
    gl_Position = VxProjMat * VxViewMat * vec4(VxVertex, 1.0);
}