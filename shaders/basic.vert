#version 460 core
layout (location = 0) in vec3 aPos;

uniform vec3 aLoc;

void main()
{
   gl_Position = vec4(aPos.x + aLoc.x, aPos.y + aLoc.y, aPos.z + aLoc.z, 1.0);
}
