#version 330 core

out vec4 FragColor;

in vec2 aLocalPosition;
in vec3 aColor;

const float smoothingEdge = 0.03f;

void main()
{

	float distance = 0.5f - length(aLocalPosition);
	float alpha = smoothstep(0.0f, smoothingEdge, distance);
	
	FragColor = vec4(aColor, alpha);
};