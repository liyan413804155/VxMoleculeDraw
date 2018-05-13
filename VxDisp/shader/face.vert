#version 330

in vec3 VxVertex;

void main()
{
    gl_Position = vec4(VxVertex, 1.0);
}