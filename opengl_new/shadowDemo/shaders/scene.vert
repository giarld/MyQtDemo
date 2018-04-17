#version 330 core

uniform mat4 u_projMatrix;
uniform mat4 u_modelMatrix;
uniform mat4 u_viewMatrix;
uniform mat4 u_biasMatrix;
uniform mat4 u_shadowMatrix;
uniform mat3 u_normalMatrix;

in vec4 a_vertex;
in vec4 a_color;
in vec3 a_normal;

out vec4 col;
out vec3 normal;
out vec3 eye;
out vec4 projCoord;

void main()
{
    col = a_color;

    mat4 shadowMartix = u_biasMatrix * u_projMatrix * u_shadowMatrix;
    vec4 vertex = u_modelMatrix * a_vertex;
    projCoord = shadowMartix * vertex;

    vertex = u_viewMatrix * vertex;
    eye = -vec3(vertex);

    normal = u_normalMatrix * a_normal;

    gl_Position = u_projMatrix * vertex;
}
