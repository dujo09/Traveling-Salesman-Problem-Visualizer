#version 330 core

out vec4 FragColor;

in vec2 aLocalPosition;
in vec3 aColor;

void main()
{

	float distance = 0.5f - length(aLocalPosition);
	float alpha = smoothstep(0.0f, 0.02f, distance);
	
	FragColor = vec4(aColor, alpha);
};