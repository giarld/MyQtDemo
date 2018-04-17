#version 330 core
uniform vec4 ambient;
uniform vec3 lightPos;
in vec4 col;
in vec3 normal;
out vec4 fragColor;
void main()
{
    vec3 L = normalize(lightPos);
    float NL = max(0.2, dot(normal, L));

    vec3 scatteredLight = vec3(ambient);
//    vec3 color = clamp(col.rgb * 0.2 + col.rgb * 0.8 * NL, 0.2, 1.0);
    vec3 color = col.rgb;
    vec3 rgb = min(color * NL * scatteredLight, vec3(1.0));
    fragColor = vec4(rgb, col.a);
}
