#version 330 core

layout(location = 0) out vec4 vFragColor;	//fragment output colour
uniform vec4 Color;
void main()
{
	vFragColor = Color;
}