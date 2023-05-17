#version 330 core

layout (location = 0) in vec2 inPosition;
layout (location = 1) in vec3 inColor;

uniform mat4 projectionMatrix;
uniform float lineWidth;

out VS_OUT 
{
    vec3 color;
	float lineWidth;
} vs_out;

void main()
{
	gl_Position = projectionMatrix * vec4(inPosition, 0.0f, 1.0f);

	vs_out.color = inColor;
	vs_out.lineWidth = lineWidth;
};

