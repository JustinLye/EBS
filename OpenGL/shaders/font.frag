#version 330 core
in vec2 TexCoords;
out vec4 Color;

uniform sampler2D text;
uniform vec3 TextColor;

void main()
{
	vec4 sampled = vec4(1.0f, 1.0f, 1.0f, texture(text, TexCoords).r);
	Color = vec4(TextColor, 1.0f) * sampled;
}