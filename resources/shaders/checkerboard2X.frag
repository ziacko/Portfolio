/*
    -- Vertex Engine X --

    Copyright 2020 UAA Software

    Permission is hereby granted, free of charge, to any person obtaining a copy of this software and
    associated documentation files (the "Software"), to deal in the Software without restriction,
    including without limitation the rights to use, copy, modify, merge, publish, distribute,
    sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in all copies or substantial
    portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT
    NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
    NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES
    OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
    CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#version 450
#extension GL_EXT_gpu_shader4 : enable
in defaultBlock
{
	vec4 position;
	vec2 uv;
} inBlock;

in lanczosBlock
{
    vec2 centerUV;
    vec2 LStep1UV;
    vec2 LStep2UV;
    vec2 LStep3UV;
    vec2 LStep4UV;
    vec2 RStep1UV;
    vec2 RStep2UV;
    vec2 RStep3UV;
    vec2 RStep4UV;
} lancBlock;

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

layout(binding = 3) uniform upsampleSettings
{
    float edgeThreshold;
    float spatialFlickerTime;
    float timeMax;
    float morphologicalStrength;
    float morphologicalSharpen;
    float timeMin;
};

layout(binding = 0) uniform sampler2D currentColor;
layout(binding = 1) uniform sampler2D previousColor;
layout(binding = 2) uniform sampler2D velocity;

layout(binding = 5) uniform downscaleSettings
{
    float       txlWidthOffset;
    float       txlHeightOffset;
	int 		downsampleMode;
};

layout(std140, binding = 6) uniform lanczosSettings
{
	float magicValue1;
	float magicValue2;
	float magicValue3;
	float magicValue4;
	float magicValue5;
};


#define VXAA_W 0
#define VXAA_E 1
#define VXAA_N 2
#define VXAA_S 3
#define VXAA_NW 0
#define VXAA_NE 1
#define VXAA_SW 2
#define VXAA_SE 3

float saturate( float x )
{
    return clamp( x, 0.0, 1.0 );
}

vec4 pow3( vec4 x, float y )
{
    return vec4( pow( x.x, y ), pow( x.y, y ), pow( x.z, y ), x.w );
}

float VXAALuma( vec3 c )
{
    return dot( c, vec3( 0.2126, 0.7152, 0.0722 ) );
}

float VXAALumaDiff( vec3 x, vec3 y )
{
    float l1 = dot( x, vec3( 0.2126, 0.7152, 0.0722 ) );
    float l2 = dot( y, vec3( 0.2126, 0.7152, 0.0722 ) );
    return abs( l1 - l2 );
}

vec3 VXAAClampHistory( vec3 history, vec4 currN[4] )
{
    vec3 cmin = min( min( currN[0].rgb, currN[1].rgb ), min( currN[2].rgb, currN[3].rgb ) );
    vec3 cmax = max( min( currN[0].rgb, currN[1].rgb ), max( currN[2].rgb, currN[3].rgb ) );
    return vec3(
        clamp( history.r, cmin.r, cmax.r ),
        clamp( history.g, cmin.g, cmax.g ),
        clamp( history.b, cmin.b, cmax.b )
    );
}

vec2 VXAADifferentialBlendWeight( vec4 n[4] )
{
    float diffWE = VXAALumaDiff( n[ VXAA_W ].rgb, n[ VXAA_E ].rgb );
    float diffNS = VXAALumaDiff( n[ VXAA_N ].rgb, n[ VXAA_S ].rgb );
    return diffWE < diffNS ? vec2( 0.5, 0.0 ) : vec2( 0.0, 0.5 );
}

vec4 VXAADifferentialBlend( vec4 n[4], vec2 w )
{
    vec4 c = vec4( 0.0 );
    c += ( n[ VXAA_W ] + n[ VXAA_E ] ) * w.x;
    c += ( n[ VXAA_N ] + n[ VXAA_S ] ) * w.y;
    return c;
}

void VXAAUpsampleT4x( out vec4 vtex[4], vec4 current, vec4 history, vec4 currN[4], vec4 histN[4] )
{
    vec4 n1[4], n2[4];
    
    n1[VXAA_W] = currN[VXAA_W];
    n1[VXAA_E] = current;
    n1[VXAA_N] = history;
    n1[VXAA_S] = histN[VXAA_S];
    
    n2[VXAA_W] = history;
    n2[VXAA_E] = histN[VXAA_E];
    n2[VXAA_N] = currN[VXAA_N];
    n2[VXAA_S] = current;
    
    
    vec4 weights = vec4( VXAADifferentialBlendWeight( n1 ), VXAADifferentialBlendWeight( n2 ) );
    vtex[VXAA_NW] = history;
    vtex[VXAA_NE] = VXAADifferentialBlend( n2, weights.zw );
    vtex[VXAA_SW] = VXAADifferentialBlend( n1, weights.xy );
    vtex[VXAA_SE] = current;
}

vec4 lanczos(vec4 current, vec4 history, vec4 currN[4], vec4 histN[4])
{
    vec4 n1[4], n2[4];
    
    n1[VXAA_W] = currN[VXAA_W];
    n1[VXAA_E] = current;
    n1[VXAA_N] = history;
    n1[VXAA_S] = histN[VXAA_S];
    
    //n2[VXAA_W] = history;
    //n2[VXAA_E] = histN[VXAA_E];
    //n2[VXAA_N] = currN[VXAA_N];
    //n2[VXAA_S] = current;

	vec4 fragColor = texture2D(currentColor, inBlock.uv) * magicValue1;
	
	fragColor += n1[VXAA_W] * magicValue2;
	fragColor += n1[VXAA_E] * magicValue3;
	fragColor += n1[VXAA_N] * magicValue4;
	fragColor += n1[VXAA_S] * magicValue5;

    //fragColor += n2[VXAA_W] * magicValue2;
	//fragColor += n2[VXAA_E] * magicValue3;
	//fragColor += n2[VXAA_N] * magicValue4;
	//fragColor += n2[VXAA_S] * magicValue5;

	return fragColor;
}

void main()
{    
    vec2 uv = gl_FragCoord.xy / resolution;
    
    // Sample scene and neighbourhood.
    
    vec4 current = clamp( vec4( texture( currentColor, inBlock.uv ).rgb, 1.0 ), vec4( 0.0f ), vec4( 1.0f ) );
    vec4 history = clamp( vec4( texture( previousColor, inBlock.uv ).rgb, 1.0 ), vec4( 0.0f ), vec4( 1.0f ) );
    current.a = VXAALuma( current.rgb ); history.a = VXAALuma( history.rgb );
    
    vec4 currN[4];
    currN[VXAA_W] = clamp( texture( currentColor, inBlock.uv + vec2( -1.0f,  0.0f ) / resolution.xy ), vec4( 0.0f ), vec4( 1.0f ) );
    currN[VXAA_E] = clamp( texture( currentColor, inBlock.uv + vec2(  1.0f,  0.0f ) / resolution.xy ), vec4( 0.0f ), vec4( 1.0f ) );
    currN[VXAA_N] = clamp( texture( currentColor, inBlock.uv + vec2(  0.0f, -1.0f ) / resolution.xy ), vec4( 0.0f ), vec4( 1.0f ) );
    currN[VXAA_S] = clamp( texture( currentColor, inBlock.uv + vec2(  0.0f, -1.0f ) / resolution.xy ), vec4( 0.0f ), vec4( 1.0f ) );
    currN[VXAA_W].a = VXAALuma( currN[ VXAA_W ].rgb );
    currN[VXAA_E].a = VXAALuma( currN[ VXAA_E ].rgb );
    currN[VXAA_N].a = VXAALuma( currN[ VXAA_N ].rgb );
    currN[VXAA_S].a = VXAALuma( currN[ VXAA_S ].rgb );
    
    vec4 histN[4];
    histN[VXAA_W] = clamp( texture( previousColor, inBlock.uv + vec2( -1.0f,  0.0f ) / resolution.xy ), vec4( 0.0f ), vec4( 1.0f ) );
    histN[VXAA_E] = clamp( texture( previousColor, inBlock.uv + vec2(  1.0f,  0.0f ) / resolution.xy ), vec4( 0.0f ), vec4( 1.0f ) );
    histN[VXAA_N] = clamp( texture( previousColor, inBlock.uv + vec2(  0.0f, -1.0f ) / resolution.xy ), vec4( 0.0f ), vec4( 1.0f ) );
    histN[VXAA_S] = clamp( texture( previousColor, inBlock.uv + vec2(  0.0f, -1.0f ) / resolution.xy ), vec4( 0.0f ), vec4( 1.0f ) );
    histN[VXAA_W].a = VXAALuma( histN[ VXAA_W ].rgb );
    histN[VXAA_E].a = VXAALuma( histN[ VXAA_E ].rgb );
    histN[VXAA_N].a = VXAALuma( histN[ VXAA_N ].rgb );
    histN[VXAA_S].a = VXAALuma( histN[ VXAA_S ].rgb );
    history.rgb = VXAAClampHistory( history.rgb, currN );

    outColor = lanczos(current, history, currN, histN);
}