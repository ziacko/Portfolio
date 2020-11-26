#version 450

layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

in defaultBlock
{
	vec4 		position;
    vec4        normal;
	vec2		uv;
} inBlock[];

in rayBlock
{
	vec4		position;
}	inRay[];

out tessBlock
{
    vec4 		position;
	vec4 		normal;
	vec2		uv;
} outBlock;

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
	uint 		totalFrames;
};

layout(binding = 0) buffer rayResults
{
    float isHit;

} result;

float epsilon = 0.3421;

void main()
{
    //go through the triangle and ray-casst from here
    for(uint iter = 0; iter < 3; iter++)
    {
        gl_Position = gl_in[iter].gl_Position;
        outBlock.position = gl_in[iter].gl_Position;
        outBlock.normal = inBlock[iter].normal;
        outBlock.uv = inBlock[iter].uv;

        //shoot a ray at the screen space location?
       
        EmitVertex();
    }

        vec4 rayStart = inverse(view) * vec4(mousePosition, 0, 1);
        vec4 rayEnd = inverse(view) * vec4(mousePosition, 1.0 - epsilon, 1.0);

        rayStart /= rayStart.w;
        rayEnd /= rayEnd.w;

        vec4 rayDirection = normalize(rayEnd - rayStart);

        vec3 N = cross(vec3(outBlock.position[1] - outBlock.position[0]), vec3(outBlock.position[2] - outBlock.position[0]));
        float d = dot(-N, vec3(outBlock.position[0]));
        vec4 L  = vec4(N.x, N.y, N.z, d);

        float intersectPoint1 = dot(L, vec4(rayStart, 1));
        float intersectPoint2 = dot(L, vec4(rayDirection, 1));

        float t = -(intersectPoint1 / intersectPoint2);

        vec3 point = rayStart + (t * rayDirection);

        if(t == 1)
        {
            result.isHit = 1;
        }

    EndPrimitive();
}