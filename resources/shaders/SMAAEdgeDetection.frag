#version 450
#define SMAA_THRESHOLD 0.05
#define SMAA_LOCAL_CONTRAST_ADAPTATION_FACTOR 2.0
#define SMAA_DEPTH_THRESHOLD (0.1 * SMAA_THRESHOLD)

in defaultBlock
{
	vec4 position;
	vec2 uv;
} inBlock;

in edgeBlock
{
	vec4 offset[3];
} inEdge;

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

layout(std140, binding = 2) uniform edgeDetectionSettings
{
    float filterLevel;
};

layout(binding = 0) uniform sampler2D colorTexture;
layout(binding = 1) uniform sampler2D depthTexture;

vec2 deltaResolution = vec2(1.0 / resolution.x, 1.0 / resolution.y );

/**
 * Luma Edge Detection
 *
 * IMPORTANT NOTICE: luma edge detection requires gamma-corrected colors, and
 * thus 'colorTex' should be a non-sRGB texture.
 */
vec2 SMAALumaEdgeDetectionPS(vec2 texcoord, vec4 offset[3], sampler2D colorTex
                               #if SMAA_PREDICATION
                               , sampler2D predicationTex
                               #endif
                               )
{
    // Calculate the threshold:
    #if SMAA_PREDICATION
    vec2 threshold = SMAACalculatePredicatedThreshold(texcoord, offset, sampler2D(predicationTex));
    #else
    vec2 threshold = vec2(SMAA_THRESHOLD, SMAA_THRESHOLD);
    #endif

    // Calculate lumas:
    vec3 weights = vec3(0.2126, 0.7152, 0.0722);
    float L = dot(texture(colorTex, texcoord).rgb, weights);

    float Lleft = dot(texture(colorTex, offset[0].xy).rgb, weights);
    float Ltop  = dot(texture(colorTex, offset[0].zw).rgb, weights);

    // We do the usual threshold:
    vec4 delta;
    delta.xy = abs(L - vec2(Lleft, Ltop));
    vec2 edges = step(threshold, delta.xy);

    // Then discard if there is no edge:
    if (dot(edges, vec2(1.0, 1.0)) == 0.0)
        discard;

    // Calculate right and bottom deltas:
    float Lright = dot(texture(colorTex, offset[1].xy).rgb, weights);
    float Lbottom  = dot(texture(colorTex, offset[1].zw).rgb, weights);
    delta.zw = abs(L - vec2(Lright, Lbottom));

    // Calculate the maximum delta in the direct neighborhood:
    vec2 maxDelta = max(delta.xy, delta.zw);

    // Calculate left-left and top-top deltas:
    float Lleftleft = dot(texture(colorTex, offset[2].xy).rgb, weights);
    float Ltoptop = dot(texture(colorTex, offset[2].zw).rgb, weights);
    delta.zw = abs(vec2(Lleft, Ltop) - vec2(Lleftleft, Ltoptop));

    // Calculate the final maximum delta:
    maxDelta = max(maxDelta.xy, delta.zw);
    float finalDelta = max(maxDelta.x, maxDelta.y);

    // Local contrast adaptation:
    edges.xy *= step(finalDelta, SMAA_LOCAL_CONTRAST_ADAPTATION_FACTOR * delta.xy);

    return edges;
}

/**
 * Color Edge Detection
 *
 * IMPORTANT NOTICE: color edge detection requires gamma-corrected colors, and
 * thus 'colorTex' should be a non-sRGB texture.
 */
vec2 SMAAColorEdgeDetectionPS(vec2 texcoord, vec4 offset[3], sampler2D colorTex
                                #if SMAA_PREDICATION
                                , sampler2D predicationTex
                                #endif
                                ) 
{
    // Calculate the threshold:
    #if SMAA_PREDICATION
    vec2 threshold = SMAACalculatePredicatedThreshold(texcoord, offset, predicationTex);
    #else
    vec2 threshold = vec2(SMAA_THRESHOLD, SMAA_THRESHOLD);
    #endif

    // Calculate color deltas:
    vec4 delta;
    vec3 C = texture(colorTex, texcoord).rgb;

    vec3 Cleft = texture(colorTex, offset[0].xy).rgb;
    vec3 t = abs(C - Cleft);
    delta.x = max(max(t.r, t.g), t.b);

    vec3 Ctop  = texture(colorTex, offset[0].zw).rgb;
    t = abs(C - Ctop);
    delta.y = max(max(t.r, t.g), t.b);

    // We do the usual threshold:
    vec2 edges = step(threshold, delta.xy);

    // Then discard if there is no edge:
    if (dot(edges, vec2(1.0, 1.0)) == 0.0)
        discard;

    // Calculate right and bottom deltas:
    vec3 Cright = texture(colorTex, offset[1].xy).rgb;
    t = abs(C - Cright);
    delta.z = max(max(t.r, t.g), t.b);

    vec3 Cbottom  = texture(colorTex, offset[1].zw).rgb;
    t = abs(C - Cbottom);
    delta.w = max(max(t.r, t.g), t.b);

    // Calculate the maximum delta in the direct neighborhood:
    vec2 maxDelta = max(delta.xy, delta.zw);

    // Calculate left-left and top-top deltas:
    vec3 Cleftleft  = texture(colorTex, offset[2].xy).rgb;
    t = abs(C - Cleftleft);
    delta.z = max(max(t.r, t.g), t.b);

    vec3 Ctoptop = texture(colorTex, offset[2].zw).rgb;
    t = abs(C - Ctoptop);
    delta.w = max(max(t.r, t.g), t.b);

    // Calculate the final maximum delta:
    maxDelta = max(maxDelta.xy, delta.zw);
    float finalDelta = max(maxDelta.x, maxDelta.y);

    // Local contrast adaptation:
    edges.xy *= step(finalDelta, SMAA_LOCAL_CONTRAST_ADAPTATION_FACTOR * delta.xy);

    return edges;
}

/**
 * Gathers current pixel, and the top-left neighbors.
 */
vec3 SMAAGatherNeighbours(vec2 texcoord, vec4 offset[3], sampler2D tex) 
{
    #ifdef SMAAGather
    return textureGather(tex, texcoord + deltaResolution * vec2(-0.5, -0.5)).grb;
    #else
    float P = texture(tex, texcoord).r;
    float Pleft = texture(tex, offset[0].xy).r;
    float Ptop  = texture(tex, offset[0].zw).r;
    return vec3(P, Pleft, Ptop);
    #endif
}

/**
 * Depth Edge Detection
 */
vec2 SMAADepthEdgeDetectionPS(vec2 texcoord, vec4 offset[3], sampler2D depthTex) 
{
    vec3 neighbours = SMAAGatherNeighbours(texcoord, offset, depthTex);
    vec2 delta = abs(neighbours.xx - vec2(neighbours.y, neighbours.z));
    vec2 edges = step(SMAA_DEPTH_THRESHOLD, delta);

    if (dot(edges, vec2(1.0, 1.0)) == 0.0)
	{
        discard;
	}

    return edges;
}

vec4 FreiChen()
{
    vec2 texel = vec2(1.0 / resolution.x, 1.0 / resolution.y);
    //if I were to put all these into imGUI it would be about 81 (9x9) uniforms!
    mat3 G[9] = 
    {
        mat3( 0.3535533845424652, 0, -0.3535533845424652, 0.5, 0, -0.5, 0.3535533845424652, 0, -0.3535533845424652 ),
        mat3( 0.3535533845424652, 0.5, 0.3535533845424652, 0, 0, 0, -0.3535533845424652, -0.5, -0.3535533845424652 ),
        mat3( 0, 0.3535533845424652, -0.5, -0.3535533845424652, 0, 0.3535533845424652, 0.5, -0.3535533845424652, 0 ),
        mat3( 0.5, -0.3535533845424652, 0, -0.3535533845424652, 0, 0.3535533845424652, 0, 0.3535533845424652, -0.5 ),
        mat3( 0, -0.5, 0, 0.5, 0, 0.5, 0, -0.5, 0 ),
        mat3( -0.5, 0, 0.5, 0, 0, 0, 0.5, 0, -0.5 ),
        mat3( 0.1666666716337204, -0.3333333432674408, 0.1666666716337204, -0.3333333432674408, 0.6666666865348816, -0.3333333432674408, 0.1666666716337204, -0.3333333432674408, 0.1666666716337204 ),
        mat3( -0.3333333432674408, 0.1666666716337204, -0.3333333432674408, 0.1666666716337204, 0.6666666865348816, 0.1666666716337204, -0.3333333432674408, 0.1666666716337204, -0.3333333432674408 ),
        mat3( 0.3333333432674408, 0.3333333432674408, 0.3333333432674408, 0.3333333432674408, 0.3333333432674408, 0.3333333432674408, 0.3333333432674408, 0.3333333432674408, 0.3333333432674408 ),
    };

    mat3 I;
    float cnv[9];
    vec3 tex;

    for(float i = 0.0; i < 3.0; i++)
    {
        for(float j = 0.0; j < 3.0; j++)
        {
            tex = texture2D(colorTexture, inBlock.uv + texel * vec2(i - 1.0, j - 1.0)).rgb;
            I[int(i)][int(j)] = length(tex);
        }
    }

    for(int i = 0; i < 9; i++)
    {
        float dp3 = dot(G[i][0], I[0]) + dot(G[i][1], I[1]) + dot(G[i][2], I[2]) * filterLevel;
        cnv[i] = dp3 * dp3;
    }

    float M = (cnv[0] + cnv[1]) + (cnv[2] + cnv[3]);
    float S = (cnv[4] + cnv[5]) + (cnv[6] + cnv[7]) + (cnv[8] + M);

    return vec4(vec3(sqrt(M/S)), 1);
}

vec4 Prewitt()
{
    float widthStep = 1.0 / resolution.x;
    float heightStep = 1.0 / resolution.y;
    vec2 offsets[9];
    vec4 samples[9];

    offsets[0] = vec2(-widthStep, -heightStep);
    offsets[1] = vec2(0.0, -heightStep);
    offsets[2] = vec2(widthStep, -heightStep);	   
    offsets[3] = vec2(-widthStep, 0.0);
    offsets[4] = vec2(0.0, 0.0);
    offsets[5] = vec2(widthStep, 0.0);	   
    offsets[6] = vec2(-widthStep, heightStep);
    offsets[7] = vec2(0.0, heightStep);
    offsets[8] = vec2(widthStep, heightStep);

    for(int sampleIndex = 0; sampleIndex < 9; sampleIndex++)
    {
        samples[sampleIndex] = texture2D(colorTexture, inBlock.uv + offsets[sampleIndex]);
    }

    vec4 horzontalEdge = samples[2] + samples[5] + samples[8] - (samples[0] + samples[3] + samples[6]);
    vec4 verticalEdge = samples[0] + samples[1] + samples[2] - (samples[6] + samples[7] + samples[8]);
    vec4 color = sqrt((horzontalEdge * horzontalEdge) + (verticalEdge * verticalEdge));

    vec4 result = vec4(0);
    if(length(color) > filterLevel)
    {
        result = vec4(1);
    }

    else
    {
        result = vec4(0);
    }

    return result;
}

void main()
{
    outColor = vec4(SMAAColorEdgeDetectionPS(inBlock.uv, inEdge.offset, colorTexture).xy, 0, 1);
}