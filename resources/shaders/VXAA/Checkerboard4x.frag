#version 450

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

layout(binding = 6) uniform upsampleSettings
{
    float edgeThreshold;//TEMPORALEDGE_THRES 0.08
    float spatialFlickerTime;//SPATIAL_FLICKER_TIME 2.75
    float timeMax;//TEMPORALEDGE_TIME_MAX 1.5
    double timeMin;//TEMPORALEDGE_TIME_MIN 0.0000001
};

layout(binding = 0) uniform sampler2D currentColor;
layout(binding = 1) uniform sampler2D previousColor;



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

void main()
{
    vec2 fcoord = gl_FragCoord.xy; fcoord.x += 400.5f;
    vec2 uv = fcoord.xy / iResolution.xy;
    
    // Sample scene and neighbourhood.
    
    vec4 current = clamp( vec4( texture( VXAA_TEXTURE_CURRENT, uv ).rgb, 1.0 ), vec4( 0.0f ), vec4( 1.0f ) );
    vec4 history = clamp( vec4( texture( VXAA_TEXTURE_PREV, uv ).rgb, 1.0 ), vec4( 0.0f ), vec4( 1.0f ) );
    current.a = VXAALuma( current.rgb ); history.a = VXAALuma( history.rgb );
    
    vec4 currN[4];
    currN[VXAA_W] = clamp( texture( VXAA_TEXTURE_CURRENT, uv + vec2( -1.0f,  0.0f ) / iResolution.xy ), vec4( 0.0f ), vec4( 1.0f ) );
    currN[VXAA_E] = clamp( texture( VXAA_TEXTURE_CURRENT, uv + vec2(  1.0f,  0.0f ) / iResolution.xy ), vec4( 0.0f ), vec4( 1.0f ) );
    currN[VXAA_N] = clamp( texture( VXAA_TEXTURE_CURRENT, uv + vec2(  0.0f, -1.0f ) / iResolution.xy ), vec4( 0.0f ), vec4( 1.0f ) );
    currN[VXAA_S] = clamp( texture( VXAA_TEXTURE_CURRENT, uv + vec2(  0.0f, -1.0f ) / iResolution.xy ), vec4( 0.0f ), vec4( 1.0f ) );
    currN[VXAA_W].a = VXAALuma( currN[ VXAA_W ].rgb );
    currN[VXAA_E].a = VXAALuma( currN[ VXAA_E ].rgb );
    currN[VXAA_N].a = VXAALuma( currN[ VXAA_N ].rgb );
    currN[VXAA_S].a = VXAALuma( currN[ VXAA_S ].rgb );
    
    vec4 histN[4];
    histN[VXAA_W] = clamp( texture( VXAA_TEXTURE_PREV, uv + vec2( -1.0f,  0.0f ) / iResolution.xy ), vec4( 0.0f ), vec4( 1.0f ) );
    histN[VXAA_E] = clamp( texture( VXAA_TEXTURE_PREV, uv + vec2(  1.0f,  0.0f ) / iResolution.xy ), vec4( 0.0f ), vec4( 1.0f ) );
    histN[VXAA_N] = clamp( texture( VXAA_TEXTURE_PREV, uv + vec2(  0.0f, -1.0f ) / iResolution.xy ), vec4( 0.0f ), vec4( 1.0f ) );
    histN[VXAA_S] = clamp( texture( VXAA_TEXTURE_PREV, uv + vec2(  0.0f, -1.0f ) / iResolution.xy ), vec4( 0.0f ), vec4( 1.0f ) );
    histN[VXAA_W].a = VXAALuma( histN[ VXAA_W ].rgb );
    histN[VXAA_E].a = VXAALuma( histN[ VXAA_E ].rgb );
    histN[VXAA_N].a = VXAALuma( histN[ VXAA_N ].rgb );
    histN[VXAA_S].a = VXAALuma( histN[ VXAA_S ].rgb );
    history.rgb = VXAAClampHistory( history.rgb, currN );
   
    
    // Temporal checkerboard upsample pass.
    vec4 vtex[4];
    VXAAUpsampleT4x( vtex, current, history, currN, histN );
    
    // Average all samples.
    //fragColor = ( vtex[VXAA_NW] + vtex[VXAA_NE] + vtex[VXAA_SW] + vtex[VXAA_SE] ) * 0.25f;
    fragColor = current;
    fragColor.b = 0.5f;
    //if( iMouse.x > fragCoord.x ) fragColor = current;
}