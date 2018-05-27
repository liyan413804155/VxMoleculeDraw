#version 330

uniform mat4 VxProjMat;
uniform mat4 VxViewMat;
uniform mat4 VxModelMat;

in vec3 VxVertex;
in vec3 VxNormal;

out vec3 VvNormal;

void main()
{
    VvNormal = mat3(VxViewMat) * mat3(VxModelMat) * VxNormal;

    gl_Position = VxProjMat * VxViewMat * VxModelMat *vec4(VxVertex, 1.0);
}