#version 330 core

layout (location = 0) in vec2 inPosition;
layout (location = 1) in vec3 inColor;
layout (location = 2) in vec2 inLocalPosition;

uniform mat4 projectionMatrix;

out vec3 aColor;
out vec2 aLocalPosition;

void main()
{
	gl_Position = projectionMatrix * vec4(inPosition, 0.0f, 1.0f);

	aColor = inColor;
	aLocalPosition = inLocalPosition;
};

