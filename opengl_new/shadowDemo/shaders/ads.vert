#version 330 core
in vec4 posAttr;
in vec4 colAttr;
out vec4 col;
uniform mat4 matrix;
uniform mat4 projMatrix;
void main()
{
    col = colAttr;
    gl_Position = projMatrix * matrix * posAttr;
}
