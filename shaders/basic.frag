#version 460 core
out vec4 FragColor;

uniform vec4 globColor;

void main()
{
    FragColor = globColor;
}
