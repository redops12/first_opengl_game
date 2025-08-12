#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCord;

uniform vec3 aLoc;
out vec2 TexCord;

void main()
{
   gl_Position = vec4(aPos.x + aLoc.x, aPos.y + aLoc.y, aPos.z + aLoc.z, 1.0);
   TexCord = aTexCord;
}
