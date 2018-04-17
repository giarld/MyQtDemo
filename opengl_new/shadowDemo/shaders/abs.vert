#version 330 core
uniform mat4 u_modelMatrix;
uniform mat4 u_viewMatrix;
uniform mat4 u_projMatrix;
in vec4 posAttr;
void main()
{
    gl_Position = u_projMatrix * u_viewMatrix * u_modelMatrix * posAttr;
}
