#version 330

uniform mat4 VxViewMat;

uniform vec3 VxColor;
uniform vec3 VxBackColor;
uniform float VxDiffuse;
uniform float VxShininess;
uniform float VxSpecular;

uniform float VxAmbient;
uniform vec3 VxLightColor;
uniform vec3 VxLightDir;

in vec3 VvNormal;

out vec4 VxOutput;

vec3 addLight
(
in vec3 faceColor,
in vec3 faceNormal,
in float faceDiffuse,
in float faceSpecular,
in float faceShininess,
in float ambient,
in vec3 lightColor,
in vec3 lightDir,
in vec3 eyeDir
);

void main ()
{
    vec3 color;
    if (gl_FrontFacing)
        color = VxColor;
    else 
        color = VxBackColor;
        
    color = addLight(color, VvNormal, VxDiffuse, VxSpecular, VxShininess, 
                        VxAmbient, VxLightColor, VxLightDir, VxViewMat[2].xyz);
    
    VxOutput = vec4(color, 0.0);
}