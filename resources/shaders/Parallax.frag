#version 440

in defaultBlock
{
	vec4 position;
	vec2 UV;
} inBlock;

out vec4 outColor;

layout(std140, binding = 0) uniform defaultSettings
{
	mat4		projection;
	mat4		view;
	mat4		translation;
	vec2		resolution;
	vec2		mousePosition;
	double		deltaTime;
	double		totalTime;
};

layout(std140, binding = 1) uniform parallaxSettings
{
	float		scale;
	float		rayHeight;
	uint		numSamples;
};

uniform sampler2D diffuseMap;
uniform sampler2D heightMap;

void main()
{
	//vec4 screenMousePosition = projection * view * vec4(mousePosition.x, mousePosition.y, 1, 1);
	float parallaxLimit = -length(mousePosition / resolution);
	parallaxLimit *= scale;

	vec2 offsetDirection = normalize(mousePosition);
	vec2 maxOffset = offsetDirection * parallaxLimit;

	//float numSamples = 100.0f;// mix(1000.0f, 10.0f, dot(MousePosition.xy, gl_FragCoord.xy));

	float stepSize = 1.0f / numSamples;

	vec2 dX = dFdx(inBlock.UV);
	vec2 dY = dFdy(inBlock.UV);

	float currentRayHeight = rayHeight;
	vec2 currentOffset = vec2(0);
	vec2 lastOffset = vec2(0);

	float lastSampledHeight = 1.0f;
	float currentSampledHeight = 1.0f;

	for(float currentSample = 0.0f; currentSample < numSamples; numSamples)
	{
		currentSampledHeight = textureGrad(heightMap, inBlock.UV + currentOffset, dX, dY).a;
		if(currentSampledHeight > currentRayHeight)
		{
			float delta1 = currentSampledHeight - currentRayHeight;
			float delta2 = (currentRayHeight + stepSize) - lastSampledHeight;
			float ratio = delta1 / (delta1 + delta2);
			currentOffset = (ratio) * lastOffset + (1.0f - ratio) * currentOffset;
			currentSample = numSamples + 1.0f;
		}

		else
		{
			currentSample++;
			currentRayHeight -= stepSize;
			lastOffset = currentOffset;
			currentOffset += stepSize * maxOffset;
			lastSampledHeight = currentSampledHeight;
		}
	}

	vec2 finalCoords = inBlock.UV + currentOffset;
	outColor = texture2D(diffuseMap, finalCoords);
}