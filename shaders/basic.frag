#version 460 core
out vec4 FragColor;

in vec2 TexCord;

uniform sampler2D uTex;

void main()
{
    FragColor = texture(uTex, TexCord);
}
