#version 330 core

in vec2 aLocalPosition;
in vec3 aColor;

out vec4 FragColor;

const float FAR_SMOOTHING_EDGE = 0.03f;

void main()
{

	float distance = 0.5f - length(aLocalPosition);
	float alpha = smoothstep(0.0f, FAR_SMOOTHING_EDGE, distance);
	
	FragColor = vec4(aColor, alpha);
};