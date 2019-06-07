#version 440
#extension GL_EXT_gpu_shader4 : enable
in defaultBlock
{
	vec4 position;
    vec4 fxaaShift;
	vec2 uv;
} inBlock;

out vec4 outColor;

layout(std140, binding = 0) uniform defaultSettings
{
	mat4		projection;
	mat4		view;
	mat4		translation;
	vec2		resolution;
	vec2		mousePosition;
	float		deltaTime;
	float		totalTime;
	float 		framesPerSecond;
	uint		totalFrames;
};

layout(std140, binding = 5) uniform fxaaSettings
{
	float subPixShift;// = 0.25;
    float vxOffset;
    float maxSpan; //8.0
    float reduceMul;//1.0/8.0
    float reduceMin;//1.0/128
};

layout(binding = 0) uniform sampler2D colorTex; // 0

vec3 FxaaPixelShader( 
  vec4 posPos, // Output of FxaaVertexShader interpolated across screen.
  sampler2D tex, // Input texture.
  vec2 rcpFrame) // Constant {1.0/frameWidth, 1.0/frameHeight}.
{   
        //color edge detection?
/*---------------------------------------------------------*/
    vec3 rgbNW = texture(tex, posPos.zw).xyz;
    vec3 rgbNE = textureOffset(tex, posPos.zw, ivec2(1,0)).xyz;
    vec3 rgbSW = textureOffset(tex, posPos.zw, ivec2(0,1)).xyz;
    vec3 rgbSE = textureOffset(tex, posPos.zw, ivec2(1,1)).xyz;
    vec3 rgbM  = texture(tex, posPos.xy).xyz;

        //luma edge detection
/*---------------------------------------------------------*/
    vec3 luma = vec3(0.299, 0.587, 0.114);
    float lumaNW = dot(rgbNW, luma);
    float lumaNE = dot(rgbNE, luma);
    float lumaSW = dot(rgbSW, luma);
    float lumaSE = dot(rgbSE, luma);
    float lumaM  = dot(rgbM,  luma);

    //luma minmax between all four directions
/*---------------------------------------------------------*/
    float lumaMin = min(lumaM, min(min(lumaNW, lumaNE), min(lumaSW, lumaSE)));
    float lumaMax = max(lumaM, max(max(lumaNW, lumaNE), max(lumaSW, lumaSE)));

    //direction to shift the UV?
/*---------------------------------------------------------*/
    vec2 dir; 
    dir.x = -((lumaNW + lumaNE) - (lumaSW + lumaSE));
    dir.y =  ((lumaNW + lumaSW) - (lumaNE + lumaSE));
/*---------------------------------------------------------*/
    float dirReduce = max(
        (lumaNW + lumaNE + lumaSW + lumaSE) * (0.25 * reduceMul),
        reduceMin);
    float rcpDirMin = 1.0/(min(abs(dir.x), abs(dir.y)) + dirReduce);
    dir = min(vec2( maxSpan,  maxSpan), 
          max(vec2(-maxSpan, -maxSpan), 
          dir * rcpDirMin)) * rcpFrame.xy;

        //can these texture lookups be optimized?
/*--------------------------------------------------------*/
    vec3 rgbA = (1.0/2.0) * (
        texture2D(tex, posPos.xy + dir * (1.0/3.0 - 0.5)).xyz +
        texture2D(tex, posPos.xy + dir * (2.0/3.0 - 0.5)).xyz);

    vec3 rgbB = rgbA * (1.0/2.0) + (1.0/4.0) * (
        texture2D(tex, posPos.xy + dir * (0.0/3.0 - 0.5)).xyz +
        texture2D(tex, posPos.xy + dir * (3.0/3.0 - 0.5)).xyz);

    float lumaB = dot(rgbB, luma);

    if((lumaB < lumaMin) || (lumaB > lumaMax))
    { 
        return rgbA;
    }
    return rgbB; 
}

vec4 PostFX(sampler2D tex, vec2 uv, float time)
{
  vec4 c = vec4(0.0);
  vec2 rcpFrame = vec2(1.0/resolution.x, 1.0/resolution.y);
  c.rgb = FxaaPixelShader(inBlock.fxaaShift, tex, rcpFrame);
  //c.rgb = 1.0 - texture2D(tex, posPos.xy).rgb;
  c.a = 1.0;
  return c;
}

void main() 
{ 
  gl_FragColor = PostFX(colorTex, inBlock.uv, deltaTime);
}