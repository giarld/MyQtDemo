#version 330 core

uniform vec3 u_lightDirection;
uniform sampler2DShadow u_shadowTexture;

in vec4 col;
in vec3 normal;
in vec3 eye;
in vec4 projCoord;

out vec4 fragColor;
void main()
{
    vec4 color;
    if(textureProj(u_shadowTexture, projCoord) < 1.0)
    {
        color = col * 0.2;
    }
    else
    {
        vec3 N = normalize(normal);
        float diffuseIntensity = max(dot(N, u_lightDirection), 0.2);
        color = color * 0.3;

        if(diffuseIntensity > 0.0) {
            color += col * diffuseIntensity;
            vec3 reflection = reflect(-u_lightDirection, N);
            vec3 E = normalize(eye);
            float specularItensity = pow(max(dot(reflection, E), 0.0), 20.0);
            color += vec4(1.0) * specularItensity;
        }
    }



    fragColor = color;
}
