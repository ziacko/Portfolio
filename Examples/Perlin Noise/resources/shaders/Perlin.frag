#version 440

in vec4 vPosition;
in vec2 vUV;

out vec4 outColor;

layout(std140, binding = 0) uniform defaultSettings
{
	mat4		projection;
	mat4		view;
	mat4		translation;
	ivec2		resolution;
	ivec2		mousePosition;
	double		deltaTime;
	double		totalTime;
};

layout(std140, binding = 1) uniform perlinSettings
{
	float		modValue;
	float		permuteValue;
	float		taylorInverse;
	float		fadeValue1;
	float		fadeValue2;
	float		fadeValue3;
	uint		numOctaves;

	float		noiseValue;//41.0
	float		noiseValue2;//2.3
	uint		colorBias;//20

	/*float		pattern1Value1;//2.5
	float		pattern1Value2;//0.4
	float		pattern1Value3;//5.2
	float		pattern1Value4;//1.3
	float		pattern1Value5;//4.0
	float		pattern1Value6;//1.7
	float		pattern1Value7;//9.2
	float		pattern1Value8;//4.0
	float		pattern1Value9;//8.3
	float		pattern1Value10;//2.8
	float		pattern1Value11;//4.0*/

	float		pattern2Value1;//0.3
	float		pattern2Value2;//0.8
	float		pattern2Value3;//5.2
	float		pattern2Value4;//1.3
	float		pattern2Value5;//4.0
	float		pattern2Value6;//1.7
	float		pattern2Value7;//9.2
	float		pattern2Value8;//4.0
	float		pattern2Value9;//8.3
	float		pattern2Value10;//2.8
	float		pattern2Value11;//4.0
};

vec4 ModifyValue(vec4 value)
{
	return value - floor(value * (1.0f / modValue)) * modValue;
}

vec4 Permute(vec4 value)
{
	return ModifyValue(((value * permuteValue) + 1.0f) * value);
}

vec4 TaylorInvertSqrt(vec4 value)
{
	return taylorInverse * value;
}

vec2 Fade(vec2 value)
{
	return value * value * value * (value * (value * fadeValue1 - fadeValue2) + fadeValue3);
}

float ClassicNoise(vec2 perlinVector)
{
	vec4 pi = floor(perlinVector.xyxy) + vec4(0.0, 0.0, 1.0, 1.0);
	vec4 perlinFractal = fract(perlinVector.xyxy) - vec4(0.0, 0.0, 1.0, 1.0);
	
	pi = ModifyValue(pi);
	vec4 iX = pi.xzxz;
	vec4 iY = pi.yyww;
	vec4 fractalX = perlinFractal.xzxz;
	vec4 fractalY = perlinFractal.yyww;

	vec4 i = Permute(Permute(iX) + iY);

	vec4 gX = fract(i * (1.0 / noiseValue)) * 2.0 - 1.0;
	vec4 gY = abs(gX) - 0.5;
	vec4 tX = floor(gX + 0.5);
	gX = gX - tX;

	vec2 g00 = vec2(gX.x, gY.x);
	vec2 g10 = vec2(gX.y, gY.y);
	vec2 g01 = vec2(gX.z, gY.z);
	vec2 g11 = vec2(gX.w, gY.w);

	vec4 normal = TaylorInvertSqrt(vec4(dot(g00, g00), dot(g01, g01), dot(g10, g10), dot(g11, g11)));

	g00 *= normal.x;
	g01 *= normal.y;
	g10 *= normal.z;
	g11 *= normal.w;

	float normal00 = dot(g00, vec2(fractalX.x, fractalY.x));
	float normal10 = dot(g10, vec2(fractalX.y, fractalY.y));
	float normal01 = dot(g01, vec2(fractalX.z, fractalY.z));
	float normal11 = dot(g11, vec2(fractalX.w, fractalY.w));

	vec2 fadeXY = Fade(perlinFractal.xy);
	vec2 normalX = mix(vec2(normal00, normal01), vec2(normal10, normal11), fadeXY.x);
	float normalXY = mix(normalX.x, normalX.y, fadeXY.y);
	
	return noiseValue2 * normalXY;
}

float PerlinNoise(vec2 perlinVector, vec2 rep)
{
	vec4 pi = floor(perlinVector.xyxy) + vec4(0.0, 0.0, 1.0, 1.0);
	vec4 perlinFractal = fract(perlinVector.xyxy) - vec4(0.0, 0.0, 1.0, 1.0);
	
	pi = mod(pi, rep.xyxy);
	pi = ModifyValue(pi);

	vec4 iX = pi.xzxz;
	vec4 iY = pi.yyww;
	vec4 fractalX = perlinFractal.xzxz;
	vec4 fractalY = perlinFractal.yyww;

	vec4 i = Permute(Permute(iX) + iY);
	vec4 gX = fract(i * (1.0 / noiseValue)) * 2.0 - 1.0;
	vec4 gY = abs(gX) - 0.5;
	vec4 tX = floor(gX + 0.5);
	gX = gX - tX;

	vec2 g00 = vec2(gX.x, gY.x);
	vec2 g10 = vec2(gX.y, gY.y);
	vec2 g01 = vec2(gX.z, gY.z);
	vec2 g11 = vec2(gX.w, gY.w);

	vec4 normal = TaylorInvertSqrt(vec4(dot(g00, g00), dot(g10, g10), dot(g01, g01), dot(g11, g11)));

	g00 *= normal.x;
	g01 *= normal.y;
	g10 *= normal.z;
	g11 *= normal.w;

	float normal00 = dot(g00, vec2(fractalX.x, fractalY.x));
	float normal10 = dot(g10, vec2(fractalX.y, fractalY.y));
	float normal01 = dot(g01, vec2(fractalX.z, fractalY.z));
	float normal11 = dot(g11, vec2(fractalX.w, fractalY.w));

	vec2 fadeXY = Fade(perlinFractal.xy);
	vec2 normalX = mix(vec2(normal00, normal01), vec2(normal10, normal11), fadeXY.x);
	float normalXY = mix(normalX.x, normalX.y, fadeXY.y);
	return noiseValue2 * normalXY;
}

float FBM(vec2 perlinVector, uint numOctaves, float lacunarity, float gain)
{
	float sum = 0.0;
	float amp = 1.0f;
	vec2 pP = perlinVector;

	int octaveIter = 0;

	for(octaveIter = 0; octaveIter < numOctaves; octaveIter++)
	{
		amp *= gain;
		sum += amp * ClassicNoise(pP);
		pP *= lacunarity;
	}

	return sum;
}

/*float Pattern(in vec2 perlinVector)
{
	float l = pattern1Value1;
	float g = pattern1Value2;
	//int NumOctaves = 10;

	vec2 q = vec2(FBM(perlinVector + vec2(0.0, 0.0), numOctaves, 1, g), FBM(perlinVector + vec2(pattern1Value3, pattern1Value4), numOctaves, 1, g));
	vec2 r = vec2(FBM(perlinVector + pattern1Value5 * q + vec2(pattern1Value6, pattern1Value7), numOctaves, 1, g), FBM(perlinVector + pattern1Value8 * q + vec2(pattern1Value9, pattern1Value10), numOctaves, 1, g));

	return FBM(perlinVector + pattern1Value11 * r, numOctaves, 1, g);
}*/

float Pattern2(in vec2 perlinVector, out vec2 q, out vec2 r, in float time)
{
	float l = pattern2Value1;
	float g = pattern2Value2;
	//int NumOctaves = 15;

	q.x = FBM(perlinVector + vec2(time, time), numOctaves, l, g);
	q.y = FBM(perlinVector + vec2(pattern2Value3 * time, pattern2Value4 * time), numOctaves, l, g);
	
	r.x = FBM(perlinVector + pattern2Value5 * q + vec2(pattern2Value6, pattern2Value7), numOctaves, l, g);
	r.y = FBM(perlinVector + pattern2Value8 * q + vec2(pattern2Value9, pattern2Value10), numOctaves, l, g);

	return FBM(perlinVector + pattern2Value11 * r, numOctaves, l, g);
}

void main()
{
	vec2 q = vUV;
	vec2 p = -1.0 + 2.0 * q;
	vec2 qQ;
	vec2 r;
	float color = Pattern2(p, qQ, r, float(totalTime)) * colorBias;

	//Color *= 3.5;

	color = color - int(color);

	outColor = vec4(color);	
}