#version 330

uniform vec3 VxColor;
uniform vec3 VxBackColor;

out vec4 VxOutput;

void main ()
{
    if (gl_FrontFacing)
        VxOutput = vec4(VxColor, 0.0);
    else 
        VxOutput = vec4(VxBackColor, 0.0);
}