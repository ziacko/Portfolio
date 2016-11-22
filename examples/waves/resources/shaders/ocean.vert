#version 420

uniform mat4 screenToCamera;
uniform mat4 cameraToWorld;
uniform mat4 worldToScreen;
uniform vec3 worldCamera;
uniform vec3 worldSunDir;

uniform vec2 gridSize;
uniform float normals;
uniform float choppy;

uniform sampler2DArray fftWavesSampler;
uniform sampler3D slopeVariancesampler;

uniform vec4 gridSizes;

out vec2 uVert;
out vec3 pVert;

vec2 OceanPos(vec4 vertex)
{
	vec3 cameraDir = normalize((screenToCamera * vertex).xyz);
	vec3 worldDir = (cameraToWorld * vec4(cameraDir, 0.f)).xyz;
	float t = -worldCamera.z / worldDir.z;
	return worldCamera.xy + t * worldDir.xy;
}

void main()
{
	uVert = OceanPos(gl_Vertex);
	vec2 ux = OceanPos(gl_Vertex + vec4(gridSize.x, 0.f, 0.f, 0.f));
	vec2 uy = OceanPos(gl_Vertex + vec4(0.f, gridSize.y, 0.f, 0.f));
	vec2 duX = abs(ux - uVert) * 2.f;
	vec2 duY = abs(uy - uVert) * 2.f;

	vec3 dP = vec3(0.f);
	dP.z += texture2DArrayGrad(fftWavesSampler, vec3(uVert / gridSizes.x, 0.f), duX / gridSizes.x, duY / gridSizes.x).x;
	dP.z += texture2DArrayGrad(fftWavesSampler, vec3(uVert / gridSizes.y, 0.f), duX / gridSizes.y, duY / gridSizes.y).y;
	dP.z += texture2DArrayGrad(fftWavesSampler, vec3(uVert / gridSizes.z, 0.f), duX / gridSizes.z, duY / gridSizes.z).z;
	dP.z += texture2DArrayGrad(fftWavesSampler, vec3(uVert / gridSizes.w, 0.f), duX / gridSizes.w, duY / gridSizes.w).w;

	if(choppy > 0.f)
	{
		dP.xy += texture2DArrayGrad(fftWavesSampler, vec3(uVert / gridSizes.x, 3.f), duX / gridSizes.x, duY / gridSizes.x).xy;
		dP.xy += texture2DArrayGrad(fftWavesSampler, vec3(uVert / gridSizes.y, 3.f), duX / gridSizes.y, duY / gridSizes.y).zw;
		dP.xy += texture2DArrayGrad(fftWavesSampler, vec3(uVert / gridSizes.z, 4.f), duX / gridSizes.z, duY / gridSizes.z).xy;
		dP.xy += texture2DArrayGrad(fftWavesSampler, vec3(uVert / gridSizes.w, 4.f), duX / gridSizes.w, duY / gridSizes.w).zw;
	}

	pVert = vec3(uVert + dP.xy, dP.z);

	gl_Position = worldToScreen * vec4(pVert, 1.f);
}
