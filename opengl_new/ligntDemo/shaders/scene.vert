#version 330 core
uniform mat4 matrix;
uniform mat4 projMatrix;
uniform mat3 normalMatrix;
in vec4 posAttr;
in vec4 colAttr;
in vec3 normalAttr;
out vec4 col;
out vec3 normal;
void main()
{
    col = colAttr;
    normal = normalize(normalMatrix * normalAttr);
    gl_Position = projMatrix * matrix * posAttr;
}
