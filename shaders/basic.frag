#version 460 core
out vec4 FragColor;

in vec3 Color;
in vec2 TexCord;

uniform sampler2D uTex;

void main()
{
    vec4 textureColor = texture(uTex, TexCord);
    FragColor = vec4(Color, textureColor.a);
}
