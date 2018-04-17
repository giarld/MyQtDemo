#version 330 core
//out vec4 color;
out float fragmentdepth;
void main()
{
//    color = vec4(1.0);
    fragmentdepth = gl_FragCoord.z;
}
