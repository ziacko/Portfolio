/** Perfect Perspective PS, version 3.5.1
All rights (c) 2018 Jakub Maksymilian Fober (the Author).

The Author provides this shader (the Work)
under the Creative Commons CC BY-NC-ND 4.0 license available online at
http://creativecommons.org/licenses/by-nc-nd/4.0/.
The Author further grants permission for reuse of screen-shots and game-play
recordings derived from the Work, provided that the reuse is for the purpose of
promoting and/or summarizing the Work or is a part of an online forum post or
social media post and that any use is accompanied by a link to the Work and a
credit to the Author. (crediting Author by pseudonym "Fubax" is acceptable)

For inquiries please contact jakub.m.fober@pm.me
For updates visit GitHub repository at
https://github.com/Fubaxiusz/fubax-shaders/

This shader version is based upon research paper by
Fober, J. M.
	Perspective picture from Visual Sphere:
	a new approach to image rasterization

	arXiv: 2003.10558 [cs.GR] (2020)
available online at https://arxiv.org/abs/2003.10558
*/


  ////////////
 /// MENU ///
////////////

#include "ReShadeUI.fxh"

uniform int FOV < __UNIFORM_SLIDER_INT1
	ui_category = "Field of View";
	ui_label = "Game field of view";
	ui_tooltip = "This setting should match your in-game FOV (in degrees)";
	#if __RESHADE__ < 40000
		ui_step = 0.2;
	#endif
	ui_min = 0; ui_max = 170;
> = 90;

uniform int Type < __UNIFORM_COMBO_INT1
	ui_category = "Field of View";
	ui_label = "Type of FOV";
	ui_tooltip = "...in stereographic mode\n"
		"If image bulges in movement (too high FOV),\n"
		"change it to 'Diagonal'.\n"
		"When proportions are distorted at the periphery\n"
		"(too low FOV), choose 'Vertical' or '4:3'.\n"
		"\nAdjust so that round objects are still round \n"
		"in the corners, and not oblong.\n"
		"\n*This method works only in 'navigation' preset,\n"
		"or k=0.5 on manual.";
	ui_items =
		"Horizontal FOV\0"
		"Diagonal FOV\0"
		"Vertical FOV\0"
		"4:3 FOV\0";
> = 0;

uniform int Projection < __UNIFORM_RADIO_INT1
	ui_spacing = 1;
	ui_category = "Perspective";
	ui_category_closed = true;
	ui_text = "Select gaming style";
	ui_label = "Perspective type";
	ui_tooltip =
		"Choose type of perspective, according to game-play style.\n"
		"For manual perspective adjustment, select the last option.\n"
		" Navigation  K  0.5  (Stereographic)\n"
		" Aiming      K  0    (Equidistant)\n"
		" Distance    K -0.5  (Equisolid)";
	ui_items =
		"Navigation\0"
		"Aiming\0"
		"Feel of distance\0"
		"\tmanual adjustment*\0";
> = 0;

uniform float K < __UNIFORM_SLIDER_FLOAT1
	ui_category = "Perspective";
	ui_label = "K (manual)*";
	ui_tooltip =
		"K  1.0 ...Rectilinear projection (standard), preserves straight lines,"
		" but doesn't preserve proportions, angles or scale.\n"
		"K  0.5 ...Stereographic projection (navigation, shape) preserves angles and proportions,"
		" best for navigation through tight spaces.\n"
		"K  0 .....Equidistant (aiming) maintains angular speed of motion,"
		" best for aiming at fast targets.\n"
		"K -0.5 ...Equisolid projection (distance) preserves area relation,"
		" best for navigation in open space.\n"
		"K -1.0 ...Orthographic projection preserves planar luminance,"
		" has extreme radial compression. Found in peephole viewer.";
	ui_min = -1.0; ui_max = 1.0;
> = 1.0;

uniform float Vertical < __UNIFORM_SLIDER_FLOAT1
	ui_spacing = 2;
	ui_category = "Perspective";
	ui_text = "Global adjustment";
	ui_label = "Vertical distortion";
	ui_tooltip = "Cylindrical perspective <<>> Spherical perspective";
	ui_min = 0.0; ui_max = 1.0; ui_step = 0.01;
> = 1.0;

uniform float VerticalScale < __UNIFORM_SLIDER_FLOAT1
	ui_category = "Perspective";
	ui_label = "Vertical proportions";
	ui_tooltip = "Adjust anamorphic correction for cylindrical perspective";
	ui_min = 0.8; ui_max = 1.0; ui_step = 0.01;
> = 0.98;

uniform bool Vignette < __UNIFORM_INPUT_BOOL1
	ui_category = "Border";
	ui_category_closed = true;
	ui_label = "use Vignette";
	ui_tooltip = "Create lens-correct natural vignette effect";
> = true;

uniform float Zooming < __UNIFORM_DRAG_FLOAT1
	ui_category = "Border";
	ui_label = "Border scale";
	ui_tooltip = "Adjust image scale and cropped area";
	ui_min = 0.5; ui_max = 2.0; ui_step = 0.001;
> = 1.0;

uniform float4 BorderColor < __UNIFORM_COLOR_FLOAT4
	ui_category = "Border";
	ui_label = "Border color";
	ui_tooltip = "Use Alpha to change transparency";
> = float4(0.027, 0.027, 0.027, 0.784);

uniform float BorderCorner < __UNIFORM_SLIDER_FLOAT1
	ui_category = "Border";
	ui_label = "Corner roundness";
	ui_tooltip = "Represents corners curvature\n0.0 gives sharp corners";
	ui_min = 1.0; ui_max = 0.0;
> = 0.062;

uniform bool MirrorBorder < __UNIFORM_INPUT_BOOL1
	ui_category = "Border";
	ui_label = "Mirrored border";
	ui_tooltip = "Choose original or mirrored image at the border";
> = true;

uniform bool DebugPreview < __UNIFORM_INPUT_BOOL1
	ui_spacing = 2;
	ui_category = "Tools for debugging";
	ui_category_closed = true;
	ui_label = "Resolution scale map";
	ui_tooltip = "Color map of the Resolution Scale:\n"
		" Red   - under-sampling\n"
		" Green - super-sampling\n"
		" Blue  - neutral sampling";
> = false;

uniform int2 ResScale < __UNIFORM_INPUT_INT2
	ui_category = "Tools for debugging";
	ui_text = "screen resolution  |  virtual, super resolution";
	ui_label = "Difference";
	ui_tooltip = "Simulates application running beyond\n"
		"native screen resolution (using VSR or DSR)";
	ui_type = "drag";
	ui_min = 16; ui_max = 16384; ui_step = 0.2;
> = int2(1920, 1920);


  ////////////////
 /// TEXTURES ///
////////////////

#include "ReShade.fxh"

// Define screen texture with mirror tiles
sampler BackBuffer
{
	Texture = ReShade::BackBufferTex;
	AddressU = MIRROR;
	AddressV = MIRROR;
};


  /////////////////
 /// FUNCTIONS ///
/////////////////

// Convert RGB to gray-scale
float grayscale(float3 Color)
{ return max(max(Color.r, Color.g), Color.b); }

// Linear pixel step function for anti-aliasing
float pixStep(float x)
{ return saturate(x/fwidth(x)); }

/*Universal perspective model by Jakub Max Fober,
Gnomonic to custom perspective variant.
This algorithm is a part of scientific paper:
	arXiv: 2003.10558 [cs.GR] (2020)
Input data:
	FOV -> Camera Field of View in degrees.
	scrCoord -> screen coordinates (from -1, to 1),
		where p(0,0) is at the center of the screen.
	k -> distortion parameter (from -1, to 1).
	l -> vertical distortion parameter (from 0, to 1).
	s -> anamorphic correction parameter (from 0.8, to 1)
Output data:
	vignette -> vignetting mask in linear space
	scrCoord -> texture lookup perspective coordinates
*/
float univPerspVignette(float k, float l, float s, in out float2 scrCoord)
{
	// Bypass
	if (FOV==0 || k==1.0 && !Vignette && (l==1.0 || s==1.0))
		return 1.0;

	// Get radius
	float R = (l==1.0)?
		length(scrCoord) : // Spherical
		length( float2(scrCoord.x, sqrt(l)*scrCoord.y) ); // Cylindrical

	// Get half field of view
	const float halfOmega = radians(FOV*0.5);
	// Radius for gnomonic projection wrapping
	const float tanHalfOmega = tan(halfOmega);

	// Get incident angle
	float theta;
	     if (k>0.0) theta = atan(tan(k*halfOmega)*R)/k;
	else if (k<0.0) theta = asin(sin(k*halfOmega)*R)/k;
	else /*k==0.0*/ theta = halfOmega*R;

	// Generate vignette
	float vignetteMask;
	if (Vignette)
	{
		// Limit FOV span, k+- in [0.5, 1] range
		float thetaLimit = max(abs(k), 0.5)*theta;
		// Create spherical vignette
		vignetteMask = lerp(
			cos(thetaLimit), // Cosine law of illumination
			rcp(pow(tan(thetaLimit), 2)+1.0), // Inverse square law
			clamp(k+0.5, 0.0, 1.0) // k in [-0.5, 0.5] range
		);
		// Cylinder vignette
		if (l!=1.0)
		{
			// Get cylinder 3D vector
			float3 perspVec;
			perspVec.xy = sin(theta)/R*scrCoord;
			perspVec.z = cos(theta);
			// Inverse square law
			vignetteMask /= dot(perspVec, perspVec);
		}
	}
	else // Bypass
		vignetteMask = 1.0;

	// Anamorphic correction for non-spherical perspective
	if (s!=1.0 && l!=1.0) scrCoord.y /= lerp(s, 1.0, l);

	// Normalize to FOV
	scrCoord /= tanHalfOmega;
	// Transform screen coordinates
	scrCoord *= tan(theta)/R;

	// Return vignette
	return vignetteMask;
}


// Get reciprocal screen aspect ratio (1/x)
#define RCP_ASPECT (BUFFER_HEIGHT*BUFFER_RCP_WIDTH)


  //////////////
 /// SHADER ///
//////////////

// Border mask shader with rounded corners
float BorderMaskPS(float2 sphCoord)
{
	float borderMask;

	if (BorderCorner == 0.0) // If sharp corners
		borderMask = max(abs(sphCoord.x), abs(sphCoord.y));
	else // If round corners
	{
		// Get coordinates for each corner
		float2 borderCoord = abs(sphCoord);
		// Correct corner aspect ratio
		if (BUFFER_ASPECT_RATIO > 1.0) // If in landscape mode
			borderCoord.x = borderCoord.x*BUFFER_ASPECT_RATIO + 1.0-BUFFER_ASPECT_RATIO;
		else if (BUFFER_ASPECT_RATIO < 1.0) // If in portrait mode
			borderCoord.y = borderCoord.y*RCP_ASPECT + 1.0-RCP_ASPECT;
		// Generate mask
		borderMask = length( max(borderCoord +BorderCorner-1.0, 0.0) ) /BorderCorner;
	}

	return pixStep(borderMask-1.0);
}


// Debug view mode shader
float3 DebugViewModePS(float3 display, float2 texCoord, float2 sphCoord)
{
	// Calculate radial screen coordinates before and after perspective transformation
	float4 radialCoord = float4(texCoord, sphCoord)*2.0 -1.0;
	// Correct vertical aspect ratio
	radialCoord.yw *= RCP_ASPECT;

	// Define Mapping color
	const float3   underSmpl = float3(1.0, 0.0, 0.2); // Red
	const float3   superSmpl = float3(0.0, 1.0, 0.5); // Green
	const float3 neutralSmpl = float3(0.0, 0.5, 1.0); // Blue

	// Calculate Pixel Size difference...
	float pixelScaleMap = fwidth( length(radialCoord.xy) );
	// ...and simulate Dynamic Super Resolution (DSR) scalar
	pixelScaleMap *= ResScale.x / (fwidth( length(radialCoord.zw) )*ResScale.y);
	pixelScaleMap -= 1.0;

	// Generate super-sampled/under-sampled color map
	float3 resMap = lerp(
		superSmpl,
		underSmpl,
		step(0.0, pixelScaleMap)
	);

	// Create black-white gradient mask of scale-neutral pixels
	pixelScaleMap = 1.0-abs(pixelScaleMap);
	pixelScaleMap = saturate(pixelScaleMap*4.0 -3.0); // Clamp to more representative values

	// Color neutral scale pixels
	resMap = lerp(resMap, neutralSmpl, pixelScaleMap);

	// Blend color map with display image
	return normalize(resMap) * (0.8*grayscale(display) +0.2);
}


// Main perspective shader pass
float3 PerfectPerspectivePS(float4 pos : SV_Position, float2 texCoord : TEXCOORD) : SV_Target
{
	// Convert FOV type..
	float FovType; switch(Type)
	{
		// Horizontal
		case 0: FovType = 1.0; break;
		// Diagonal
		case 1: FovType = sqrt(RCP_ASPECT*RCP_ASPECT +1.0); break;
		// Vertical
		case 2: FovType = RCP_ASPECT; break;
		// Horizontal 4:3
		case 3: FovType = 4.0/3.0 * RCP_ASPECT; break;
	}

	// Convert UV to centered coordinates
	float2 sphCoord = texCoord*2.0 -1.0;
	// Aspect Ratio correction
	sphCoord.y *= RCP_ASPECT;
	// Zoom in image and adjust FOV type (pass 1 of 2)
	sphCoord *= clamp(Zooming, 0.5, 2.0) / FovType; // Anti-cheat clamp

	// Choose projection type
	float k; switch (Projection)
	{
		case 0:  k =  0.5; break; // Stereographic
		case 1:  k =  0.0; break; // Equidistant
		case 2:  k = -0.5; break; // Equisolid
		// Manual perspective
		default: k = clamp(K, -1.0, 1.0); break;
	}
	// Perspective transform and create vignette
	float vignetteMask = univPerspVignette(k, Vertical, VerticalScale, sphCoord);

	// FOV type (pass 2 of 2)
	sphCoord *= FovType;

	// Aspect Ratio back to square
	sphCoord.y *= BUFFER_ASPECT_RATIO;

	// Outside border mask with Anti-Aliasing
	float borderMask = BorderMaskPS(sphCoord);

	// Back to UV Coordinates
	sphCoord = sphCoord*0.5 +0.5;

	// Sample display image
	float3 display = tex2D(BackBuffer, sphCoord).rgb;

	// Mask outside-border pixels or mirror
	display = lerp(
		Vignette? // Apply Vignette with sRGB gamma
			display*pow(abs(vignetteMask), rcp(2.2)) : display,
		lerp(
			MirrorBorder?
				display : tex2D(BackBuffer, texCoord).rgb,
			BorderColor.rgb,
			BorderColor.a
		), borderMask);

	// Output type choice
	if (DebugPreview) return DebugViewModePS(display, texCoord, sphCoord);
	else return display;
}


  //////////////
 /// OUTPUT ///
//////////////

technique PerfectPerspective <
	ui_label = "Perfect Perspective";
	ui_tooltip = "Adjust perspective for distortion-free picture\n"
		"(fish-eye, panini shader and vignetting)"; >
{
	pass
	{
		VertexShader = PostProcessVS;
		PixelShader = PerfectPerspectivePS;
	}
}
