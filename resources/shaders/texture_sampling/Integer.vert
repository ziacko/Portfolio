#version 420

layout (location = 0) in vec4 position;
layout (location = 1) in vec2 uv;

out defaultBlock
{
	vec4 position;
	vec2 uv;
} outBlock;

out vec4 outColor;

layout(std140, binding = 0) uniform defaultSettings
{
	mat4		projection;
	mat4 		view;
	mat4 		translation;
	vec2		resolution;
	vec2		mousePosition;
	float		deltaTime;
	float		totalTime;
	float 		framesPerSecond;
	uint 		totalFrames;
};

layout(std140, binding = 2) uniform integerSettings
{
	vec2 inSize;
    vec2 outSize;
};

uniform sampler2D defaultTexture;

vec2 targetRatio = vec2(16, 9);

vec2 calculateScalingRatio(vec2 screenSize, vec2 imageSize, vec2 targetAspectRatio)
{
	float _TargetAspectRatio = targetAspectRatio.x / targetAspectRatio.y;
	float imageAspectRatio = imageSize.x / imageSize.y;

	vec2 maxIntRatio = floor(screenSize / imageSize);

	if (imageAspectRatio == _TargetAspectRatio)
	{
		float ratio = max(min(maxIntRatio.x, maxIntRatio.y), 1.0f);
		return vec2(ratio);
	}

	vec2 maxOutputSize = imageSize * maxIntRatio;
	float maxAspectRatio = maxOutputSize.x / maxOutputSize.y;
	vec2 scalingRatio = vec2(0.0, 0.0);
	// If the ratio MA is lower than the target aspect ratio TA
	if (maxAspectRatio < _TargetAspectRatio)
	{
		scalingRatio.x = maxIntRatio.x;
		float AUH = maxOutputSize.x / _TargetAspectRatio;

		float yUpperScaleFactor = ceil(AUH / imageSize.y);
		float yLowerScaleFactor = floor(AUH / imageSize.y);

		float upperAspectRatio = maxOutputSize.x / (yUpperScaleFactor * imageSize.y);
		float lowerAspectRatio = maxOutputSize.x / (yLowerScaleFactor * imageSize.y);

		float upperTargetError = abs(_TargetAspectRatio - upperAspectRatio);
		float lowerTargetError = abs(_TargetAspectRatio - lowerAspectRatio);

		if (abs(upperTargetError - lowerTargetError) < 0.001)
		{
			float upperImageError = abs(imageAspectRatio - upperAspectRatio);
			float lowerImageError = abs(imageAspectRatio - lowerAspectRatio);
			if (upperImageError < lowerImageError)
				scalingRatio.y = yUpperScaleFactor;
			else
				scalingRatio.y = yLowerScaleFactor;
		}
		// Added an extra check in here to prefer an aspect ratio above 1.0.
		// TODO: This will need to be looked at again for aspect ratios other than 4:3
		else if (lowerTargetError < upperTargetError || upperAspectRatio < 1.0)
			scalingRatio.y = yLowerScaleFactor;
		else
			scalingRatio.y = yUpperScaleFactor;
	}
	// If the ratio MA is greater than the target aspect ratio TA
	else if (maxAspectRatio > _TargetAspectRatio)
	{
		scalingRatio.y = maxIntRatio.y;
		float AUW = maxOutputSize.y * _TargetAspectRatio;

		float xUpperScaleFactor = ceil(AUW / imageSize.x);
		float xLowerScaleFactor = floor(AUW / imageSize.x);

		float upperAspectRatio = (xUpperScaleFactor * imageSize.x) / maxOutputSize.y;
		float lowerAspectRatio = (xLowerScaleFactor * imageSize.x) / maxOutputSize.y;

		float upperTargetError = abs(_TargetAspectRatio - upperAspectRatio);
		float lowerTargetError = abs(_TargetAspectRatio - lowerAspectRatio);

		if (abs(upperTargetError - lowerTargetError) < 0.001)
		{
			float upperImageError = abs(imageAspectRatio - upperAspectRatio);
			float lowerImageError = abs(imageAspectRatio - lowerAspectRatio);
			if (upperImageError < lowerImageError)
				scalingRatio.x = xUpperScaleFactor;
			else
				scalingRatio.x = xLowerScaleFactor;
		}
		// Added an extra check in here to prefer an aspect ratio above 1.0.
		// TODO: This will need to be looked at again for aspect ratios other than 4:3
		else if (upperTargetError < lowerTargetError || lowerAspectRatio < 1.0)
			scalingRatio.x = xUpperScaleFactor;
		else
			scalingRatio.x = xLowerScaleFactor;
	}
	// If the ratio MA is equal to the target aspect ratio TA
	else
		scalingRatio = maxIntRatio;

	if (scalingRatio.x < 1.0)
		scalingRatio.x = 1.0;
	if (scalingRatio.y < 1.0)
		scalingRatio.y = 1.0;

	return scalingRatio;
}

void main()
{
	outBlock.position = projection * view * translation * position;

	vec2 box_scale = calculateScalingRatio(outSize, inSize, targetRatio);
	vec2 scale = (outSize / inSize) / box_scale;
	vec2 middle = vec2(0.5);
	vec2 TexCoord = outBlock.position.xy * 0.5f + 0.5f;
	vec2 diff = (TexCoord - middle) * scale;

	outBlock.uv = TexCoord;//middle + diff;
    gl_Position = outBlock.position;
}