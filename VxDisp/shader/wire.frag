#version 330

uniform vec3 VxColor;

out vec4 VxOutput;

void main ()
{
    VxOutput = vec4(VxColor, 0.0);
}