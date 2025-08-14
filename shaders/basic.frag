#version 460 core
out vec4 FragColor;

in vec3 Color;
in vec2 TexCord;

// uniform sampler2D uTex;
in vec3 Normal;

void main()
{
    // vec4 textureColor = texture(uTex, TexCord);
    // FragColor = vec4(Color, textureColor.a);
    FragColor = vec4(Normal * 0.5 + 0.5, 1.0);
}
