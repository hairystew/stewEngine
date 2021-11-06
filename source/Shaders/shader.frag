#version 450

//output write
layout (location = 0) in vec3 color;
layout (location = 0) out vec4 outFragColor;

void main() 
{
	//return red
	outFragColor = vec4(color ,1.0f);
}