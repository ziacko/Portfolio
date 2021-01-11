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

#define VXAA_W 0
#define VXAA_E 1
#define VXAA_N 2
#define VXAA_S 3
#define VXAA_NW 0
#define VXAA_NE 1
#define VXAA_SW 2
#define VXAA_SE 3

#define VXAA_TEMPORALEDGE_TIME_MIN 0.0000001

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

float VXAATemporalContrast( float currentLuma, float historyLuma )
{
    float x = saturate( abs( historyLuma - currentLuma ) - edgeThreshold );
    float x2 = x * x, x3 = x2 * x;
    return saturate( 3.082671957671837 * x - 3.9384920634917364 * x2 + 1.8518518518516354 * x3 );
}

float VXAAMorphStrengthShaper( float x )
{
    return 1.3 * x - 0.3 * x * x;
}

float VXAASpatialContrast( vec2 spatialLumaMinMax )
{
    float spatialContrast = spatialLumaMinMax.y - spatialLumaMinMax.x;
    return mix( 0.0f, 1.0f, spatialContrast );
}

float VXAATemporalFilterAlpha( float fpsRcp, float convergenceTime )
{
    return exp( -fpsRcp / convergenceTime );
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

vec4 VXAASharpen( vec4 history, vec4 histN[4] )
{
    vec4 nh = histN[VXAA_NW] + histN[VXAA_NE] + histN[VXAA_SW] + histN[VXAA_SE];
    return mix( history, history * 5.0f - nh, morphologicalSharpen );
}

vec4 VXAAMorphological( vec2 uv, vec4 current, vec4 currN[4], float strength )
{
    if ( strength < 0.1f ) return current;
    float lumaNW = currN[VXAA_NW].a, lumaNE = currN[VXAA_NE].a,
        lumaSW = currN[VXAA_SW].a, lumaSE = currN[VXAA_SE].a;
    lumaNE += 0.0025;
    float lumaMin = min( current.a, min( min( lumaNW, lumaNE ), min( lumaSW, lumaSE ) ) );
    float lumaMax = max( current.a, max( max( lumaNW, lumaNE ), max( lumaSW, lumaSE ) ) );
    
    vec2 dir;
    dir.x = ( lumaSW - lumaNE ) + ( lumaSE - lumaNW );
    dir.y = ( lumaSW - lumaNE ) - ( lumaSE - lumaNW );
    vec2 dirN = normalize( dir );
    
    vec4 n1 = texture( currentColor, uv - dirN * strength / resolution.xy );
    vec4 p1 = texture( currentColor, uv + dirN * strength / resolution.xy );
    return ( n1 + p1 ) * 0.5;
}

vec4 VXAAFilmic( vec2 uv, vec4 current, vec4 history, vec4 currN[4], vec4 histN[4] )
{
    // Temporal contrast weight.
    float temporalContrastWeight = VXAATemporalContrast( current.a, history.a );

    // Spatial contrast weight.
    vec2 spatialLumaMinMaxC = vec2(
        min( min( currN[0].a, currN[1].a ), min( currN[2].a, currN[3].a ) ),
        max( max( currN[0].a, currN[1].a ), max( currN[2].a, currN[3].a ) )
    );
    vec2 spatialLumaMinMaxH = vec2(
        min( min( histN[0].a, histN[1].a ), min( histN[2].a, histN[3].a ) ),
        max( max( histN[0].a, histN[1].a ), max( histN[2].a, histN[3].a ) )
    );
    float spatialContrastWeightC = VXAASpatialContrast( spatialLumaMinMaxC );
    float spatialContrastWeightH = VXAASpatialContrast( spatialLumaMinMaxH );
    float spatialContrastWeight = abs( spatialContrastWeightC - spatialContrastWeightH );
    
    // Evaluate convergence time from weights.
    float convergenceTime = mix( VXAA_TEMPORALEDGE_TIME_MIN, timeMax, temporalContrastWeight );
    convergenceTime = mix( convergenceTime, spatialFlickerTime, spatialContrastWeight );
    //float alpha = VXAATemporalFilterAlpha( deltaTime, convergenceTime );
    float alpha = VXAATemporalFilterAlpha( 1.0f / 60.0f, convergenceTime );
    
    // Apply morpholigical AA filter and sharpen.
    float strength = VXAAMorphStrengthShaper( spatialContrastWeightC * 4.0 ) * morphologicalStrength;
    current = VXAAMorphological( uv, current, currN, strength );
    current = VXAASharpen( current, currN );
    
    // Clamp history to neighbourhood, and apply filmic blend.
    history.rgb = VXAAClampHistory( history.rgb, currN );
    current = mix( current, history, alpha );
    return current;
}

void main()
{
    vec2 uv =  gl_FragCoord.xy / resolution;
    
    // Sample scene and neighbourhood.
    
    vec4 current = clamp( vec4( texture( currentColor, uv ).rgb, 1.0 ), vec4( 0.0f ), vec4( 1.0f ) );
    vec4 history = clamp( vec4( texture( previousColor, uv ).rgb, 1.0 ), vec4( 0.0f ), vec4( 1.0f ) );
    current.a = VXAALuma( current.rgb ); history.a = VXAALuma( history.rgb );
    
    vec4 currN[4];
    currN[VXAA_NW] = clamp( texture( currentColor, uv + 0.6f * vec2( -1.0f,  1.0f ) / resolution.xy ), vec4( 0.0f ), vec4( 1.0f ) );
    currN[VXAA_NE] = clamp( texture( currentColor, uv + 0.6f * vec2(  1.0f,  1.0f ) / resolution.xy ), vec4( 0.0f ), vec4( 1.0f ) );
    currN[VXAA_SW] = clamp( texture( currentColor, uv + 0.6f * vec2( -1.0f, -1.0f ) / resolution.xy ), vec4( 0.0f ), vec4( 1.0f ) );
    currN[VXAA_SE] = clamp( texture( currentColor, uv + 0.6f * vec2(  1.0f, -1.0f ) / resolution.xy ), vec4( 0.0f ), vec4( 1.0f ) );
    currN[VXAA_NW].a = VXAALuma( currN[VXAA_NW].rgb );
    currN[VXAA_NE].a = VXAALuma( currN[VXAA_NE].rgb );
    currN[VXAA_SW].a = VXAALuma( currN[VXAA_SW].rgb );
    currN[VXAA_SE].a = VXAALuma( currN[VXAA_SE].rgb );
    
    vec4 histN[4];
    histN[VXAA_NW] = clamp( texture( previousColor, uv + 0.6f * vec2( -1.0f,  1.0f ) / resolution.xy ), vec4( 0.0f ), vec4( 1.0f ) );
    histN[VXAA_NE] = clamp( texture( previousColor, uv + 0.6f * vec2(  1.0f,  1.0f ) / resolution.xy ), vec4( 0.0f ), vec4( 1.0f ) );
    histN[VXAA_SW] = clamp( texture( previousColor, uv + 0.6f * vec2( -1.0f, -1.0f ) / resolution.xy ), vec4( 0.0f ), vec4( 1.0f ) );
    histN[VXAA_SE] = clamp( texture( previousColor, uv + 0.6f * vec2(  1.0f, -1.0f ) / resolution.xy ), vec4( 0.0f ), vec4( 1.0f ) );
    histN[VXAA_NW].a = VXAALuma( histN[VXAA_NW].rgb );
    histN[VXAA_NE].a = VXAALuma( histN[VXAA_NE].rgb );
    histN[VXAA_SW].a = VXAALuma( histN[VXAA_SW].rgb );
    histN[VXAA_SE].a = VXAALuma( histN[VXAA_SE].rgb );
    
    
    // Filmic pass.    
    outColor = VXAAFilmic( uv, current, history, currN, histN );
    //outColor = current;
    //if( iMouse.x > fragCoord.x ) fragColor = current;
}