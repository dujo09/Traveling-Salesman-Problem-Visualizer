#version 330 core

layout (location = 0) in vec2 inPosition;
layout (location = 1) in vec3 inColor;

out VS_OUT {
    vec3 color;
} vs_out;

uniform mat4 projectionMatrix;

void main()
{
	gl_Position = projectionMatrix * vec4(inPosition, 0.0f, 1.0f);
	vs_out.color = inColor;
};

