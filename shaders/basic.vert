#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCord;

uniform vec3 aLoc;
out vec3 Color;
out vec2 TexCord;

void main()
{
   gl_Position = vec4(aPos.x + aLoc.x, aPos.y + aLoc.y, aPos.z + aLoc.z, 1.0);
   Color = aColor;
   TexCord = aTexCord;
}
