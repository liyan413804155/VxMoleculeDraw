#version 330


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
)
{
    vec3 color = ambient * faceColor;
    
    float dotN = dot(faceNormal, normalize(lightDir));
    if (dotN > 0)
    {
        color += dotN * faceDiffuse * lightColor;
    }
    
    float dotE = -dot(faceNormal, normalize(eyeDir + lightDir));
    if (dotE > 0)
    {
        color += dotE * faceSpecular * pow(dotE, faceShininess);
    }
    
    return color;
}